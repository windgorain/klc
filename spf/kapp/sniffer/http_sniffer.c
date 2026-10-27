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
#include "sniffer_tool.h"

static char g_http_sniffer_kp_output[256];
static char g_http_sniffer_kp_input[256];
static int g_http_sniffer_kp_output_ok = 0;
static int g_http_sniffer_kp_input_ok = 0;
static SNIFFER_PORTS_S g_http_sniffer_ports;

static int _http_sniffer_get_host(void *payload, void *payload_end, OUT LSTR_S *host)
{
    void * host_begin = strnstr(payload, "\r\nHost:", payload_end - payload);
    if (! host_begin) {
        return -1;
    }
    
    host_begin += 2;

    void * host_end = strnstr(host_begin, "\r\n", payload_end - host_begin);
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

    if (! ArrayBit_Test(g_http_sniffer_ports.port_bitmap, tcp_hdr->usDstPort)) {
        return;
    }

    void *payload = (void*)tcp_hdr + TCP_HEAD_LEN(tcp_hdr);

    int ret = _http_sniffer_get_host(payload, data_end, &host);
    if (ret < 0) { 
        return;
    }

    _sniffer_print(&host, dir);
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
    int ret = klc_register_kprobe(g_http_sniffer_kp_output, sizeof(g_http_sniffer_kp_output),
            "ip_output", _http_sniffer_handler_output, _http_sniffer_event_handler_post);

    if (ret < 0) {
        printf("SPF/http-sniffer: register kprobe failed, ret=%d\n", ret);
        return ret;
    }

    g_http_sniffer_kp_output_ok = 1;

    return 0;
}

static int _http_sniffer_init_input(void)
{
    int ret = klc_register_kprobe(g_http_sniffer_kp_input, sizeof(g_http_sniffer_kp_input),
            "tcp_v4_rcv", _http_sniffer_handler_input, _http_sniffer_event_handler_post);

    if (ret < 0) {
        printf("SPF/http-sniffer: register kprobe failed, ret=%d\n", ret);
        return ret;
    }

    g_http_sniffer_kp_input_ok = 1;

    return 0;
}

static int _http_sniffer_event_init(void)
{
    ArrayBit_Set(g_http_sniffer_ports.port_bitmap, htons(80));

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

SEC(".spf.cmd/cmd_main")
int _http_sniffer_cmd_main(int argc, char **argv, char *output_buf, int buf_size)
{
    return sniffer_cmd_main(argc, argv, output_buf, buf_size, &g_http_sniffer_ports);
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

