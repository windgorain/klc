/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-14
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_NF_H_
#define _KLC_SYM_NF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NF_DROP 
    #define NF_DROP 0
    #define NF_ACCEPT 1
    #define NF_STOLEN 2
    #define NF_QUEUE 3
    #define NF_REPEAT 4
    #define NF_STOP 5	
    #define NF_MAX_VERDICT NF_STOP
    
    enum {
    	NFPROTO_UNSPEC =  0,
    	NFPROTO_INET   =  1,
    	NFPROTO_IPV4   =  2,
    	NFPROTO_ARP    =  3,
    	NFPROTO_NETDEV =  5,
    	NFPROTO_BRIDGE =  7,
    	NFPROTO_IPV6   = 10,
    	NFPROTO_DECNET = 12,
    	NFPROTO_NUMPROTO,
    };
    
    enum nf_inet_hooks {
    	NF_INET_PRE_ROUTING,
    	NF_INET_LOCAL_IN,
    	NF_INET_FORWARD,
    	NF_INET_LOCAL_OUT,
    	NF_INET_POST_ROUTING,
    	NF_INET_NUMHOOKS,
    	NF_INET_INGRESS = NF_INET_NUMHOOKS,
    };
#endif

struct net;
struct nf_hook_ops;

typedef struct {
    U64 data[16];
}KLC_NF_REG_S;

static inline int klc_init_nf_hook_ops(KLC_NF_REG_S *reg, void *func, U8 pf, U32 hooknum, int pri)
{
    KLC_PARAM_S p;

    p.param[0] = (long)(void*)reg;
    p.param[1] = (long)(void*)func;
    p.param[2] = pf;
    p.param[3] = hooknum;
    p.param[4] = pri;

    return ulc_call_sym(-1, klc_init_nf_hook_ops, &p);
}


static inline int nf_register_net_hooks(struct net *net, const KLC_NF_REG_S *reg, unsigned int n)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)net; p1 |= (((U64)(U32)(long)reg) << 32);
        return ulc_call_sym(-1, nf_register_net_hooks, p1, n);
    } else {
        return ulc_call_sym(-1, nf_register_net_hooks, net, reg, n);
    }
}

static inline void nf_unregister_net_hooks(struct net *net, const KLC_NF_REG_S *reg, unsigned int n)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)net; p1 |= (((U64)(U32)(long)reg) << 32);
        ulc_call_sym(0, nf_unregister_net_hooks, p1, n);
    } else {
        ulc_call_sym(0, nf_unregister_net_hooks, net, reg, n);
    }
}

static inline void * klc_nf_priv(void *priv, void *skb, void *state)
{
    if (klc_is_sys32()) {
        return (void*)(((U64)priv) & 0xffffffff);
    }
    
    return priv;
}

static inline void * klc_nf_skb(void *priv, void *skb, void *state)
{
    if (klc_is_sys32()) {
        return (void*)(((U64)priv) >> 32);
    }
    
    return skb;
}

static inline void * klc_nf_state(void *priv, void *skb, void *state)
{
    if (klc_is_sys32()) {
        return (void*)(((U64)skb) & 0xffffffff);
    }
    
    return state;
}

#ifdef __cplusplus
}
#endif
#endif 
