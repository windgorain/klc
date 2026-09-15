/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-15
* Description: 
******************************************************************************/
#ifndef _KLC_DRIVER_H_
#define _KLC_DRIVER_H_

#include "klc/klc_def.h"

#ifdef __cplusplus
extern "C" {
#endif

struct net_device; 

static inline struct net_device * alloc_etherdev_mqs(int sizeof_priv, unsigned int txqs, unsigned int rxqs)
{
    return (void*)(long)ulc_call_sym(0, alloc_etherdev_mqs, sizeof_priv, txqs, rxqs);
}

static inline int klc_init_netdev(struct net_device *dev, char *name, const KLC_NET_DEV_OPS_S *ops, OUT void *netdev_ops)
{
    return ulc_call_sym(-1, klc_init_netdev, dev, name, ops, netdev_ops);
}


static inline void netif_start_queue(struct net_device *dev)
{
    ulc_call_sym(-1, klc_netif_start_queue, dev);
}


static inline void netif_stop_queue(struct net_device *dev)
{
    ulc_call_sym(-1, klc_netif_stop_queue, dev);
}

static inline int register_netdev(struct net_device *dev)
{
    return ulc_call_sym(-1, register_netdev, dev);
}

static inline void unregister_netdev(struct net_device *dev)
{
    ulc_call_sym(0, unregister_netdev, dev);
}

static inline void free_netdev(struct net_device *dev)
{
    ulc_call_sym(0, free_netdev, dev);
}

#ifdef __cplusplus
}
#endif
#endif 
