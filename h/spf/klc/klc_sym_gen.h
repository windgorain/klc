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

#ifdef __cplusplus
}
#endif
#endif 
