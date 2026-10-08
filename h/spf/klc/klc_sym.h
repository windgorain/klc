/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-29
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_H_
#define _KLC_SYM_H_

#include "klc/klc_def.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pt_regs;
struct sk_buff;

static inline int klc_init_kprobe(KLC_KPROBE_PARAM_S *p)
{
    return ulc_call_sym(-1, klc_init_kprobe, p);
}

static inline int register_kprobe(void *kp)
{
    return ulc_call_sym(-1, register_kprobe, kp);
}

static inline void unregister_kprobe(void *kp)
{
    ulc_call_sym(0, unregister_kprobe, kp);
}

static inline int klc_get_pt_params(void *p, struct pt_regs *regs, OUT  KLC_PT_PARAM_S *param)
{
    if (ulc_sys_ptr_size() == 4) {
        regs = (void*)(LONG)(((U64)(LONG)p) >> 32);
    }

    return ulc_call_sym(-1, klc_get_pt_params, regs, param);
}

static inline void klc_set_pt_rc(struct pt_regs *regs, long long rc)
{
    ulc_call_sym(-1, klc_set_pt_rc, regs, rc);
}

static inline void klc_compute_data_pointers(void *skb, void *tc)
{
    ulc_call_sym(0, klc_compute_data_pointers, skb, tc);
}

static inline int klc_get_skb_info(struct sk_buff *skb, OUT KLC_SKB_INFO_S *info)
{
    return ulc_call_sym(-1, klc_get_skb_info, skb, info);
}

static inline void klc_free_skb(void *skb)
{
    ulc_call_sym(0, klc_free_skb, skb);
}

static inline BOOL_T pskb_may_pull(struct sk_buff *skb, unsigned int len)
{
    return ulc_call_sym(FALSE, klc_pskb_may_pull, skb, len);
}

static inline void * skb_put(struct sk_buff *skb, unsigned int len)
{
    return (void*)(long)ulc_call_sym(0, klc_skb_put, (long)skb, len);
}

#ifdef __cplusplus
}
#endif
#endif 
