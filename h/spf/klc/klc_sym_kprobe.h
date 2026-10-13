/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-12
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_KPROBE_H_
#define _KLC_SYM_KPROBE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct pt_regs;

static inline int klc_register_kprobe(void *kp, int kp_size, char *sym, void *pre_handler, void *post_handler)
{
    KLC_KPROBE_PARAM_S p = {0};

    p.kp = kp;
    p.kp_size = kp_size;
    p.name = sym;
    p.pre_handler = pre_handler;
    p.post_handler = post_handler;

    return ulc_call_sym(-1, klc_register_kprobe, &p);
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

#ifdef __cplusplus
}
#endif
#endif 
