/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang
* Description: 
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

SEC("xdp")
int test_xdp_input(struct xdp_md *ctx)
{
    KLCHLP_License();

    void *data = (void*)(long)ctx->data;
    void *data_end = (void*)(long)ctx->data_end;

    long ret = KLCHLP_NameRun("namefunc_test", -1, data, data_end);
    if (ret < 0) {
        BPF_Print("Name func error");
        return XDP_PASS;
    }

    BPF_Print("namefunc pkt bytes=%llu", ret);

    KLCHLP_IDRun(TEST_ID, -1, data, data_end);
    if (ret < 0) {
        BPF_Print("ID func error");
        return XDP_PASS;
    }

    BPF_Print("idfunc pkt bytes=%llu", ret);

    return XDP_PASS;
}

char _license[] SEC("license") = "Dual BSD/GPL";

