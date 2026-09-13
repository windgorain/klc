/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang
* Description: 展示如何使用klc function支持循环, 以及如何查找字符串
* History:     
******************************************************************************/
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include "utl/types.h"
#include "utl/int_types.h"
#include "klc/klc_def.h"
#include "spf/klc/klc_help.h"
#include "klc/klc_utl.h"
#include "spf/spf_def.h"

#define TEST_ID 10001022

static int g_print_count_limit = 0;

static inline void _test_xdp_count_bytes(void *data, void *data_end)
{
    
    if (g_print_count_limit > 2) {
        return;
    }

    g_print_count_limit ++;

    long ret = KLCHLP_NameRun("namefunc_test", -1, data, data_end);
    if (ret < 0) {
        BPF_Print("Name func error");
        return;
    }

    BPF_Print("namefunc pkt bytes=%llu", ret);

    KLCHLP_IDRun(TEST_ID, -1, data, data_end);
    if (ret < 0) {
        BPF_Print("ID func error");
        return;
    }

    BPF_Print("idfunc pkt bytes=%llu", ret);
}

static inline void _test_xdp_print_host(void *data, void *data_end)
{
    char host[64];
    int ret = KLCHLP_NameRun("find_host", -1, data, data_end, host, sizeof(host));
    if (ret == 0) {
        BPF_Print("%s", host);
    }
}

SEC("xdp")
int test_xdp_input(struct xdp_md *ctx)
{
    KLCHLP_License();

    void *data = (void*)(long)ctx->data;
    void *data_end = (void*)(long)ctx->data_end;

    _test_xdp_count_bytes(data, data_end);
    _test_xdp_print_host(data, data_end);

    return XDP_PASS;
}

char _license[] SEC("license") = "Dual BSD/GPL";

