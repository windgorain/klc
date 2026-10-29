/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-19
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_NETDEV_H_
#define _KLC_SYM_NETDEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

enum {
	NETDEV_TX_OK	 = 0x00,	
	NETDEV_TX_BUSY	 = 0x10,	
};

#define NET_XMIT_SUCCESS    0x00
#define NET_XMIT_DROP       0x01	
#define NET_XMIT_CN         0x02	
#define NET_XMIT_MASK       0x0f	

enum netdev_priv_flags {
	IFF_802_1Q_VLAN			= 1<<0,
	IFF_EBRIDGE			= 1<<1,
	IFF_BONDING			= 1<<2,
	IFF_ISATAP			= 1<<3,
	IFF_WAN_HDLC			= 1<<4,
	IFF_XMIT_DST_RELEASE		= 1<<5,
	IFF_DONT_BRIDGE			= 1<<6,
	IFF_DISABLE_NETPOLL		= 1<<7,
	IFF_MACVLAN_PORT		= 1<<8,
	IFF_BRIDGE_PORT			= 1<<9,
	IFF_OVS_DATAPATH		= 1<<10,
	IFF_TX_SKB_SHARING		= 1<<11,
	IFF_UNICAST_FLT			= 1<<12,
	IFF_TEAM_PORT			= 1<<13,
	IFF_SUPP_NOFCS			= 1<<14,
	IFF_LIVE_ADDR_CHANGE		= 1<<15,
	IFF_MACVLAN			= 1<<16,
	IFF_XMIT_DST_RELEASE_PERM	= 1<<17,
	IFF_L3MDEV_MASTER		= 1<<18,
	IFF_NO_QUEUE			= 1<<19,
	IFF_OPENVSWITCH			= 1<<20,
	IFF_L3MDEV_SLAVE		= 1<<21,
	IFF_TEAM			= 1<<22,
	IFF_RXFH_CONFIGURED		= 1<<23,
	IFF_PHONY_HEADROOM		= 1<<24,
	IFF_MACSEC			= 1<<25,
	IFF_NO_RX_HANDLER		= 1<<26,
	IFF_FAILOVER			= 1<<27,
	IFF_FAILOVER_SLAVE		= 1<<28,
	IFF_L3MDEV_RX_HANDLER		= 1<<29,
	IFF_NO_ADDRCONF			= 1ULL <<30,
	IFF_TX_SKB_NO_LINEAR		= 1ULL <<31
};

struct net_device; 
struct netdev_queue;





static inline struct net_device * dev_get_by_name(struct net *net, const char *name)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)net; p1 |= (((U64)(U32)(long)name) << 32);
        return (void*)(long)ulc_call_sym(0, dev_get_by_name, p1);
    } else {
        return (void*)(long)ulc_call_sym(0, dev_get_by_name, net, name);
    }
}


static inline void dev_put(struct net_device *dev)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)dev;
        ulc_call_sym(0, dev_put, p1);
    } else {
        ulc_call_sym(0, dev_put, dev, NULL);
    }
}


static inline int klc_netdev_get_info(struct net_device *dev, OUT KLC_NETDEV_INFO_S *info)
{
    return ulc_call_sym(-1, klc_netdev_get_info, dev, info);
}


struct netdev_queue * netdev_get_tx_queue(const struct net_device *dev, U32 index)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)dev; p1 |= (((U64)index) << 32);
        return (void*)(long)ulc_call_sym(0, netdev_get_tx_queue, p1);
    } else {
        return (void*)(long)ulc_call_sym(0, netdev_get_tx_queue, dev, index);
    }
}




static inline int netdev_start_xmit(struct sk_buff *skb, struct net_device *dev, struct netdev_queue *txq, BOOL_T more)
{
    static int (*netdev_start_xmit_fn)(void *, void *, void *, BOOL_T) = NULL;
    if (!netdev_start_xmit_fn) {
        netdev_start_xmit_fn = ulc_sys_get_sym("netdev_start_xmit");
        if (! netdev_start_xmit_fn) {
            return NETDEV_TX_BUSY;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)(U32)(long)dev) << 32);
        U64 p2 = (U32)(long)txq; p2 |= (((U64)(U32)more) << 32);
        int (*netdev_start_xmit_fn32)(U64, U64) = (void*)netdev_start_xmit_fn;
        return netdev_start_xmit_fn32(p1, p2);
    } else {
        return netdev_start_xmit_fn(skb, dev, txq, more);
    }
}



static inline int dev_queue_xmit(struct sk_buff *skb)
{
    static int (*dev_queue_xmit_fn)(void *) = NULL;
    if (!dev_queue_xmit_fn) {
        dev_queue_xmit_fn = ulc_sys_get_sym("dev_queue_xmit");
        if (! dev_queue_xmit_fn) {
            kfree_skb(skb);
            return NET_XMIT_DROP;
        }
    }

    return dev_queue_xmit_fn(skb);
}


static inline BOOL_T dev_xmit_complete(int rc)
{
    if (rc < NET_XMIT_MASK) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL_T netif_xmit_frozen_or_drv_stopped(const struct netdev_queue *dev_queue)
{
    static BOOL_T (*netif_xmit_frozen_or_drv_stopped_fn)(const void *) = NULL;
    if (!netif_xmit_frozen_or_drv_stopped_fn) {
        netif_xmit_frozen_or_drv_stopped_fn = ulc_sys_get_sym("netif_xmit_frozen_or_drv_stopped");
        if (! netif_xmit_frozen_or_drv_stopped_fn) {
            return FALSE;
        }
    }

    return netif_xmit_frozen_or_drv_stopped_fn(dev_queue);
}

void klc_hard_tx_lock(struct net_device *dev, void *txq)
{
    static void (*klc_hard_tx_lock_fn)(void *, void *) = NULL;
    if (!klc_hard_tx_lock_fn) {
        klc_hard_tx_lock_fn = ulc_sys_get_sym("klc_hard_tx_lock");
        if (! klc_hard_tx_lock_fn) {
            return;
        }
    }

    klc_hard_tx_lock_fn(dev, txq);
}

void klc_hard_tx_unlock(struct net_device *dev, void *txq)
{
    static void (*klc_hard_tx_unlock_fn)(void *, void *) = NULL;
    if (!klc_hard_tx_unlock_fn) {
        klc_hard_tx_unlock_fn = ulc_sys_get_sym("klc_hard_tx_unlock");
        if (! klc_hard_tx_unlock_fn) {
            return;
        }
    }

    klc_hard_tx_unlock_fn(dev, txq);
}

#ifdef __cplusplus
}
#endif
#endif 
