/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-14
* Description: 
******************************************************************************/
#include "utl/ulc_user.h"
#include "bpf/tcp_bpf.h"
#include "spf/spf_string.h"
#include "spf/core/spf_module.h"
#include "spf/klc/klc_skb.h"
#include "spf/klc/klc_driver.h"

static void * _g_self = NULL;
static void *g_snull_netdev;


static int _snull_open(void *dev)
{
    netif_start_queue(dev); 
    return 0;
}


static int _snull_stop(void *dev)
{
    netif_stop_queue(dev); 
    return 0;
}


static int _snull_xmit(void *skb, void *dev)
{
    klc_free_skb(skb);
    return 0;
}


static const KLC_NET_DEV_OPS_S g_snull_netdev_ops = {
    .ndo_open       = _snull_open,
    .ndo_stop       = _snull_stop,
    .ndo_start_xmit = _snull_xmit,
};

static unsigned char g_snull_netdev_opsbuf[1024];


static int _snull_init(void)
{
    int ret;

    
    g_snull_netdev = alloc_etherdev_mqs(0, 1, 1);
    if (! g_snull_netdev) {
        printf("Error: Alloc netdev failed");
        return -1;
    }

    if (klc_init_netdev(g_snull_netdev, ulc_get_self_name(_g_self), &g_snull_netdev_ops, g_snull_netdev_opsbuf) < 0) {
        printf("Error: Init netdev failed");
        return -1;
    }

    
    ret = register_netdev(g_snull_netdev);
    if (ret != 0) {
        printf("Error: Reg netdev failed");
        ulc_call_sym(0, free_netdev, g_snull_netdev);
        return ret;
    }

    return 0;
}


static int _snull_exit(void)
{
    unregister_netdev(g_snull_netdev);
    free_netdev(g_snull_netdev);
    return 0;
}

SEC(SPF_SEC_EVENT)
int event(U32 event, void *data)
{
    switch (event) {
        case SPF_EVENT_SET_SELF: _g_self = data; return 0;
        case SPF_EVENT_INIT: return _snull_init();
        case SPF_EVENT_FIN1: return _snull_exit();
    }
    return 0;
}

