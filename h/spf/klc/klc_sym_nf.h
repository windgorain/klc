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

struct net;
struct nf_hook_ops;

static inline int klc_init_nf_hook_ops(KLC_PARAM_S *p)
{
    return ulc_call_sym(-1, klc_init_nf_hook_ops, p);
}


static inline int nf_register_net_hooks(struct net *net, const struct nf_hook_ops *reg, unsigned int n)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)net; p1 |= (((U64)(U32)(long)reg) << 32);
        return ulc_call_sym(-1, nf_register_net_hooks, p1, n);
    } else {
        return ulc_call_sym(-1, nf_register_net_hooks, net, reg, n);
    }
}

static inline void nf_unregister_net_hooks(struct net *net, const struct nf_hook_ops *reg, unsigned int n)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)net; p1 |= (((U64)(U32)(long)reg) << 32);
        ulc_call_sym(0, nf_unregister_net_hooks, p1, n);
    } else {
        ulc_call_sym(0, nf_unregister_net_hooks, net, reg, n);
    }
}

#ifdef __cplusplus
}
#endif
#endif 
