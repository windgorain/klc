/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"
#include "bpf/tcp_bpf.h"
#include "spf/spf_string.h"
#include "spf/core/spf_module.h"
#include "spf/klc/klc_help.h"

static char g_http_sniffer_event_kp_in[256];
static int g_http_sniffer_event_kp_ok = 0;

static void (*pf_klc_get_pt_params)(void *regs, OUT void *p);
static void* (*pf_skb_push)(void *skb, unsigned int len);
static void* (*pf_skb_pull)(void *skb, unsigned int len);
static void (*pf_compute_data_pointers)(void *skb, void *tc);
static void (*pf_klc_get_skb_info)(void *skb, OUT KLC_SKB_INFO_S *info);

static inline void _http_sniffer_print(LSTR_S *str)
{
    char info[128] = "";

    int copy_len = MIN(sizeof(info)-1, str->uiLen);

    _memcpy(info, str->pcData, copy_len);

    BPF_Print("%s", info);
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

static void _http_sniffer_process(TCP_HEAD_S *tcp_hdr, void *data_end)
{
    LSTR_S host = {0};

    if (tcp_hdr->usDstPort != htons(80)) {
        return;
    }

    void *payload = (void*)tcp_hdr + TCP_HEAD_LEN(tcp_hdr);

    int ret = _http_sniffer_get_host(payload, data_end, &host);
    if (ret < 0) { return; }

    _http_sniffer_print(&host);
}

static void _http_sniffer_ip_output(void *skb)
{
    KLC_SKB_INFO_S skbinfo;

    pf_klc_get_skb_info(skb, &skbinfo);
    KLCHLP_SkbContinue(skb, skbinfo.len);
    pf_klc_get_skb_info(skb, &skbinfo);

    int data_len = skbinfo.head_len;
    void *data = (void*)(long)skbinfo.data;
    void *data_end = data + data_len;

    TCP_HEAD_S *tcp_hdr = TCPBPF_GetTcpHeaderFromIP(data, data_end);
    if (tcp_hdr) {
        _http_sniffer_process(tcp_hdr, data_end);
    }
}

static int _http_sniffer_event_handler_in_pre(void *p, void *regs)
{
    KLC_PT_PARAM_S param = {0};

    pf_klc_get_pt_params(regs, &param);

    void *skb = (void*)(long)param.param[2];
    if (! skb) {
        return 0;
    }

    _http_sniffer_ip_output(skb);

    return 0;
}

static void _http_sniffer_event_handler_post(void *p, void *regs, unsigned long flags)
{
}

static int _http_sniffer_event_init_probe(void)
{
    KLC_KPROBE_PARAM_S p = {0};

    p.name = "ip_output";
    p.pre_handler = _http_sniffer_event_handler_in_pre;
    p.post_handler = _http_sniffer_event_handler_post;

    int ret = ulc_call_sym(-1, klc_init_kprobe, g_http_sniffer_event_kp_in, sizeof(g_http_sniffer_event_kp_in), &p);
    if (ret < 0) {
        printf("SPF: http sniffer init error: init failed, ret=%d\n", ret);
        return ret;
    }

    ret = ulc_call_sym(-1, register_kprobe, g_http_sniffer_event_kp_in);
    if (ret < 0) {
        printf("SPF: http sniffer init error: init failed, ret=%d\n", ret);
        return ret;
    }

    g_http_sniffer_event_kp_ok = 1;

    return 0;
}

static int _http_sniffer_event_init(void)
{
    pf_klc_get_pt_params = ulc_sys_get_sym("klc_get_pt_params");
    pf_compute_data_pointers = ulc_sys_get_sym("klcko_compute_data_pointers");
    pf_skb_push = ulc_sys_get_sym("skb_push");
    pf_skb_pull = ulc_sys_get_sym("skb_pull");
    pf_klc_get_skb_info = ulc_sys_get_sym("klc_get_skb_info");

    if ((! pf_klc_get_pt_params)
            || (! pf_compute_data_pointers)
            || (! pf_klc_get_skb_info)
            || (! pf_skb_push)
            || (! pf_skb_pull)) {
        printf("SPF: http sniffer init error\n");
        return -1;
    }

    return _http_sniffer_event_init_probe();
}

static int _http_sniffer_event_fini1(void)
{
    if (g_http_sniffer_event_kp_ok) {
        g_http_sniffer_event_kp_ok = 0;
        ulc_call_sym(0, unregister_kprobe, g_http_sniffer_event_kp_in);
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

