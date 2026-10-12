/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"
#include "utl/array_bit_opt.h"
#include "bpf/tcp_bpf.h"
#include "spf/spf_string.h"
#include "spf/core/spf_module.h"
#include "spf/klc/klc_help.h"
#include "spf/klc/klc_sym.h"

#define DIR_IN  0
#define DIR_OUT 1

static char g_http_sniffer_kp_output[256];
static char g_http_sniffer_kp_input[256];
static int g_http_sniffer_kp_output_ok = 0;
static int g_http_sniffer_kp_input_ok = 0;
static U32 g_http_sniffer_port[2048]; 

static inline void _http_sniffer_print(LSTR_S *str, int dir)
{
    char info[128] = "";

    int copy_len = MIN(sizeof(info)-1, str->uiLen);

    _memcpy(info, str->pcData, copy_len);

    char *dir_str = dir == DIR_IN ? "Input" : "Output";
    BPF_Print("%s: %s", dir_str, info);
}

static int _http_sniffer_get_host(void *payload, void *payload_end, OUT LSTR_S *host)
{
    void * host_begin = _strnstr(payload, "\r\nHost:", payload_end - payload);
    if (! host_begin) {
        return -1;
    }
    
    host_begin += 2;

    void * host_end = _strnstr(host_begin, "\r\n", payload_end - host_begin);
    if (! host_end) {
        return -1;
    }

    host->pcData = host_begin;
    host->uiLen = host_end - host_begin;

    return 0;
}

static void _http_sniffer_process(TCP_HEAD_S *tcp_hdr, void *data_end, int dir)
{
    LSTR_S host = {0};

    if (! ArrayBit_Test(g_http_sniffer_port, tcp_hdr->usDstPort)) {
        return;
    }

    void *payload = (void*)tcp_hdr + TCP_HEAD_LEN(tcp_hdr);

    int ret = _http_sniffer_get_host(payload, data_end, &host);
    if (ret < 0) { 
        return;
    }

    _http_sniffer_print(&host, dir);
}

static void _http_sniffer_process_pkt(void *skb, int dir)
{
    KLC_SKB_INFO_S skbinfo;
    TCP_HEAD_S *tcp_hdr;

    if (klc_get_skb_info(skb, &skbinfo) < 0) {
        return;
    }

    pskb_may_pull(skb, MIN(skbinfo.len, 1024));

    if (klc_get_skb_info(skb, &skbinfo) < 0) {
        return;
    }

    int data_len = skbinfo.head_len;
    void *data = (void*)(long)skbinfo.data;
    void *data_end = data + data_len;

    if (dir == DIR_OUT) {
        tcp_hdr = TCPBPF_GetTcpHeaderFromIP(data, data_end);
    } else {
        tcp_hdr = TCPBPF_GetTcpHeader(data, data_end);
    }

    if (tcp_hdr) {
        _http_sniffer_process(tcp_hdr, data_end, dir);
    }
}

static int _http_sniffer_handler_output(void *p, void *regs)
{
    KLC_PT_PARAM_S param = {0};

    if (klc_get_pt_params(p, regs, &param) < 0) {
        return 0;
    }

    void *skb = (void*)(long)param.param[2];
    if (! skb) {
        return 0;
    }

    _http_sniffer_process_pkt(skb, DIR_OUT);

    return 0;
}

static int _http_sniffer_handler_input(void *p, void *regs)
{
    KLC_PT_PARAM_S param = {0};

    if (klc_get_pt_params(p, regs, &param) < 0) {
        return 0;
    }

    void *skb = (void*)(long)param.param[0];
    if (! skb) {
        return 0;
    }

    _http_sniffer_process_pkt(skb, DIR_IN);

    return 0;
}

static void _http_sniffer_event_handler_post(void *p, void *regs, unsigned long flags)
{
}

static int _http_sniffer_init_output(void)
{
    KLC_KPROBE_PARAM_S p = {0};

    p.kp = g_http_sniffer_kp_output;
    p.kp_size = sizeof(g_http_sniffer_kp_output);
    p.name = "ip_output";
    p.pre_handler = _http_sniffer_handler_output;
    p.post_handler = _http_sniffer_event_handler_post;

    int ret = klc_init_kprobe(&p);
    if (ret < 0) {
        printf("SPF/http-sniffer: kprobe init failed, ret=%d\n", ret);
        return ret;
    }

    ret = register_kprobe(g_http_sniffer_kp_output);
    if (ret < 0) {
        printf("SPF/http-sniffer: register kprobe failed, ret=%d\n", ret);
        return ret;
    }

    g_http_sniffer_kp_output_ok = 1;

    return 0;
}

static int _http_sniffer_init_input(void)
{
    KLC_KPROBE_PARAM_S p = {0};

    p.kp = g_http_sniffer_kp_input;
    p.kp_size = sizeof(g_http_sniffer_kp_input);
    p.name = "tcp_v4_rcv";
    p.pre_handler = _http_sniffer_handler_input;
    p.post_handler = _http_sniffer_event_handler_post;

    int ret = klc_init_kprobe(&p);
    if (ret < 0) {
        printf("SPF/http-sniffer: kprobe init failed, ret=%d\n", ret);
        return ret;
    }

    ret = register_kprobe(g_http_sniffer_kp_input);
    if (ret < 0) {
        printf("SPF/http-sniffer: register kprobe failed, ret=%d\n", ret);
        return ret;
    }

    g_http_sniffer_kp_input_ok = 1;

    return 0;
}

static int _http_sniffer_event_init(void)
{
    ArrayBit_Set(g_http_sniffer_port, htons(80));

    int ret = _http_sniffer_init_input();
    ret |= _http_sniffer_init_output();

    return ret;
}

static int _http_sniffer_event_fini1(void)
{
    if (g_http_sniffer_kp_output_ok) {
        g_http_sniffer_kp_output_ok = 0;
        unregister_kprobe(g_http_sniffer_kp_output);
    }

    if (g_http_sniffer_kp_input_ok) {
        g_http_sniffer_kp_input_ok = 0;
        unregister_kprobe(g_http_sniffer_kp_input);
    }

    return 0;
}

static void _http_sniffer_show_ports(OUT char *buf, int buf_size)
{
    int i;
    int len;
    int count = 0;

    len = snprintf(buf, buf_size, "Sniffer ports:\n");

    for (i=1; i<65536; i++) {
        if (buf_size < len + 50) {
            break;
        }

        if (ArrayBit_Test(g_http_sniffer_port, htons(i))) {
            count ++;
            len += snprintf(buf + len, buf_size - len, " %u", i);
            if (count % 8 == 0) {
                len += snprintf(buf + len, buf_size - len, "\n");
            }
        }
    }

    snprintf(buf + len, buf_size - len, "\n");
}

static void _http_sniffer_add_ports(U16 min_port, U16 max_port)
{
    for (int i=min_port; i<=max_port; i++) {
        ArrayBit_Set(g_http_sniffer_port, htons(i));
    }
}

static void _http_sniffer_set_ports(U16 min_port, U16 max_port)
{
    memset(g_http_sniffer_port, 0, sizeof(g_http_sniffer_port));
    _http_sniffer_add_ports(min_port, max_port);
}

static void _http_sniffer_del_ports(U16 min_port, U16 max_port)
{
    for (int i=min_port; i<=max_port; i++) {
        ArrayBit_Clr(g_http_sniffer_port, htons(i));
    }
}

static void _http_sniffer_cmd_help(char *buf, int buf_size)
{
    snprintf(buf, buf_size, "Usage: \n"
            "http_sniffer show port\n"
            "http_sniffer add port min max\n"
            "http_sniffer del port min max\n"
            "http_sniffer set port min max\n");
}

static void _http_sniffer_set_port(int argc, char **argv, char *output_buf, int buf_size)
{
    char *min_port_str = (argc > 3) ? argv[3] : "0";
    char *max_port_str = (argc > 4) ? argv[4] : "0";
    U16 min_port = atoi(min_port_str);
    U16 max_port = atoi(max_port_str);

    if (max_port < min_port) {
        U16 tmp = min_port;
        min_port = max_port;
        max_port = tmp;
    }

    _http_sniffer_set_ports(min_port, max_port);
}

static void _http_sniffer_add_port(int argc, char **argv, char *output_buf, int buf_size)
{
    char *min_port_str = (argc > 3) ? argv[3] : "0";
    char *max_port_str = (argc > 4) ? argv[4] : "0";
    U16 min_port = atoi(min_port_str);
    U16 max_port = atoi(max_port_str);

    if (max_port < min_port) {
        U16 tmp = min_port;
        min_port = max_port;
        max_port = tmp;
    }

    _http_sniffer_add_ports(min_port, max_port);
}

static void _http_sniffer_del_port(int argc, char **argv, char *output_buf, int buf_size)
{
    char *min_port_str = (argc > 3) ? argv[3] : "0";
    char *max_port_str = (argc > 4) ? argv[4] : "0";
    U16 min_port = atoi(min_port_str);
    U16 max_port = atoi(max_port_str);

    if (max_port < min_port) {
        U16 tmp = min_port;
        min_port = max_port;
        max_port = tmp;
    }

    _http_sniffer_del_ports(min_port, max_port);
}

SEC(".spf.cmd/cmd_main")
int _http_sniffer_cmd_main(int argc, char **argv, char *output_buf, int buf_size)
{
    if (argc < 3) {
        _http_sniffer_cmd_help(output_buf, buf_size);
        return -1;
    }

    if ((strcmp(argv[1], "show")== 0) && (strcmp(argv[2], "port") == 0)) {
        _http_sniffer_show_ports(output_buf, buf_size);
    } else if ((strcmp(argv[1], "set")== 0) && (strcmp(argv[2], "port") == 0)) {
        _http_sniffer_set_port(argc, argv, output_buf, buf_size);
    } else if ((strcmp(argv[1], "add")== 0) && (strcmp(argv[2], "port") == 0)) {
        _http_sniffer_add_port(argc, argv, output_buf, buf_size);
    } else if ((strcmp(argv[1], "del")== 0) && (strcmp(argv[2], "port") == 0)) {
        _http_sniffer_del_port(argc, argv, output_buf, buf_size);
    } else {
        _http_sniffer_cmd_help(output_buf, buf_size);
        return -1;
    }

    return 0;
}

SEC(SPF_SEC_EVENT)
int event(U32 event)
{
    switch (event) {
        case SPF_EVENT_INIT:
            return _http_sniffer_event_init();
        case SPF_EVENT_FIN1:
            return _http_sniffer_event_fini1();
    }
    return 0;
}

SPF_MOD_COMMON_DEF(1)

