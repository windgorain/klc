/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-11-01
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_TM_H_
#define _KLC_SYM_TM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int tm_sec;    
    int tm_min;    
    int tm_hour;   
    int tm_mday;   
    int tm_mon;    
    int tm_year32; 
    long tm_year;  
    int tm_wday;   
    int tm_yday;   
}KLC_TIME_S;

struct timespec64 {
	U64   tv_sec;			
	long  tv_nsec;		
};

static inline void ktime_get_real_ts64(struct timespec64 *ts)
{
    static void (*ktime_get_real_ts64_fn)(void*) = NULL;
    if (! ktime_get_real_ts64_fn) {
        ktime_get_real_ts64_fn = ulc_sys_get_sym("ktime_get_real_ts64");
        if (! ktime_get_real_ts64_fn) {
            return;
        }
    }

    ktime_get_real_ts64_fn(ts);
}

static inline void time64_to_tm(U64 totalsecs, int offset, KLC_TIME_S *result)
{
    static void (*time64_to_tm_fn)(U64, int, void*) = NULL;
    if (! time64_to_tm_fn) {
        time64_to_tm_fn = ulc_sys_get_sym("time64_to_tm");
        if (! time64_to_tm_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = totalsecs; 
        U64 p2 = (U32)offset; p2 |= (((U64)result) << 32);
        void (*time64_to_tm_fn32)(U64, U64) = (void*)time64_to_tm_fn;
        time64_to_tm_fn32(p1, p2);
        result->tm_wday = result->tm_year;
        result->tm_yday = (result->tm_year >> 32);
        result->tm_year = (U32)result->tm_year32;
    } else {
        time64_to_tm_fn(totalsecs, offset, result);
    }
}

#ifdef __cplusplus
}
#endif
#endif 
