/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-12
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_GEN_H_
#define _KLC_SYM_GEN_H_

#ifdef __cplusplus
extern "C" {
#endif


static inline BOOL_T klc_is_sys32(void)
{
    static int sys_ptr_size = 0;

    if (sys_ptr_size == 0) {
        sys_ptr_size = ulc_sys_ptr_size();
    }

    if (sys_ptr_size == 4) {
        return TRUE;
    }

    return FALSE;
}


static inline void klc_compute_data_pointers(void *skb, void *tc)
{
    ulc_call_sym(0, klc_compute_data_pointers, skb, tc);
}


static inline S64 ktime_get_coarse_with_offset(int type)
{
    return ulc_call_sym(0, ktime_get_coarse_with_offset, type);
}


static inline U32 klc_gfp_flags(U32 type)
{
    static U32 flags[KLC_GFP_MAX] = {0};

    if (! flags[0]) {
        flags[KLC_GFP_ATOMIC] = ulc_call_sym(0, klc_gfp_flags, KLC_GFP_ATOMIC);
        flags[KLC_GFP_NOWAIT] = ulc_call_sym(0, klc_gfp_flags, KLC_GFP_NOWAIT);
        flags[KLC_GFP_KERNEL] = ulc_call_sym(0, klc_gfp_flags, KLC_GFP_KERNEL);
    }

    if (type < KLC_GFP_MAX) {
        return flags[type];
    }

    return 0;
}

static inline void local_bh_disable(void)
{
    static int (*local_bh_disable_fn)(void) = NULL;
    if (! local_bh_disable_fn) {
        local_bh_disable_fn = ulc_sys_get_sym("local_bh_disable");
        if (! local_bh_disable_fn) {
            return;
        }
    }

    local_bh_disable_fn();
}

static inline void local_bh_enable(void)
{
    static int (*local_bh_enable_fn)(void) = NULL;
    if (! local_bh_enable_fn) {
        local_bh_enable_fn = ulc_sys_get_sym("local_bh_enable");
        if (! local_bh_enable_fn) {
            return;
        }
    }

    local_bh_enable_fn();
}

static inline BOOL_T need_resched(void)
{
    static BOOL_T (*need_resched_fn)(void) = NULL;
    if (! need_resched_fn) {
        need_resched_fn = ulc_sys_get_sym("klc_need_resched");
        if (! need_resched_fn) {
            return TRUE;
        }
    }

    return need_resched_fn();
}

static inline void schedule(void)
{
    static void (*schedule_fn)(void) = NULL;
    if (! schedule_fn) {
        schedule_fn = ulc_sys_get_sym("schedule");
        if (! schedule_fn) {
            return;
        }
    }

    schedule_fn();
}

static inline BOOL_T try_to_freeze(void)
{
    static BOOL_T (*try_to_freeze_fn)(void) = NULL;
    if (! try_to_freeze_fn) {
        try_to_freeze_fn = ulc_sys_get_sym("klc_try_to_freeze");
        if (! try_to_freeze_fn) {
            return FALSE;
        }
    }

    return try_to_freeze_fn();
}

#ifdef __cplusplus
}
#endif
#endif 
