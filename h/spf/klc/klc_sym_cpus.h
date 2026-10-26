/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-20
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_CPUS_H_
#define _KLC_SYM_CPUS_H_

#ifdef __cplusplus
extern "C" {
#endif

static inline int klc_num_cpus(int type)
{
    return ulc_call_sym(0, klc_num_cpus, type);
}

static inline int klc_walk_cpus(void (*func)(int), int type)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)func; p1 |= (((U64)(U32)type) << 32);
        return ulc_call_sym(0, klc_walk_cpus, p1);
    } else {
        return ulc_call_sym(0, klc_walk_cpus, func, type);
    }
}

#ifdef __cplusplus
}
#endif
#endif 
