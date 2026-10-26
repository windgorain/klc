/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-19
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_IPOUTPUT_H_
#define _KLC_SYM_IPOUTPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

static inline int ip_local_out(struct net *net, struct sock *sk, struct sk_buff *skb)
{
    static int (*ip_local_out_fn)(void *, void *, void *) = NULL;
    if (! ip_local_out_fn) { ip_local_out_fn = ulc_sys_get_sym("ip_local_out"); }
    if (! ip_local_out_fn) { 
        kfree_skb(skb);
        return -1;
    }

    if (klc_is_sys32()) {
        U64 p1 = (U64)(U32)(long)net; p1 |= (((U64)(U32)(long)sk) << 32);
        int (*ip_local_out_fn32)(U64, void*) = (void*)ip_local_out_fn;
        return ip_local_out_fn32(p1, skb);
    } else {
        return ip_local_out_fn(net, sk, skb);
    }
}

#ifdef __cplusplus
}
#endif
#endif 
