/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-27
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_REFCOUNT_H_
#define _KLC_SYM_REFCOUNT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct refcount_struct  refcount_t;

static inline void refcount_set(refcount_t *r, int n)
{
    static void (*refcount_set_fn)(void *, int) = NULL;
    if (! refcount_set_fn) { 
        refcount_set_fn = ulc_sys_get_sym("refcount_set");
        if (! refcount_set_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)r; p1 |= (((U64)(U32)n) << 32);
        void * (*refcount_set_fn32)(U64) = (void*)refcount_set_fn;
        refcount_set_fn32(p1);
    } else {
        refcount_set_fn(r, n);
    }
}

static inline void refcount_add(int i, refcount_t *r)
{
    static void (*refcount_add_fn)(int, void*) = NULL;
    if (! refcount_add_fn) { 
        refcount_add_fn = ulc_sys_get_sym("refcount_add");
        if (! refcount_add_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)i; p1 |= (((U64)(U32)(long)r) << 32);
        void * (*refcount_add_fn32)(U64) = (void*)refcount_add_fn;
        refcount_add_fn32(p1);
    } else {
        refcount_add_fn(i, r);
    }
}


static inline BOOL_T refcount_add_not_zero(int i, refcount_t *r)
{
    static BOOL_T (*refcount_add_not_zero_fn)(int, void*) = NULL;
    if (! refcount_add_not_zero_fn) { 
        refcount_add_not_zero_fn = ulc_sys_get_sym("refcount_add_not_zero");
        if (! refcount_add_not_zero_fn) {
            return FALSE;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)i; p1 |= (((U64)(U32)(long)r) << 32);
        BOOL_T (*refcount_add_not_zero_fn32)(U64) = (void*)refcount_add_not_zero_fn;
        return refcount_add_not_zero_fn32(p1);
    } else {
        return refcount_add_not_zero_fn(i, r);
    }
}

static inline BOOL_T refcount_inc_not_zero(refcount_t *r)
{
    return refcount_add_not_zero(1, r);
}


static inline BOOL_T refcount_sub_and_test(int i, refcount_t *r)
{
    static BOOL_T (*refcount_sub_and_test_fn)(int, void*) = NULL;
    if (! refcount_sub_and_test_fn) { 
        refcount_sub_and_test_fn= ulc_sys_get_sym("refcount_sub_and_test");
        if (! refcount_sub_and_test_fn) {
            return FALSE;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)i; p1 |= (((U64)(U32)(long)r) << 32);
        BOOL_T (*refcount_sub_and_test_fn32)(U64) = (void*)refcount_sub_and_test_fn;
        return refcount_sub_and_test_fn32(p1);
    } else {
        return refcount_sub_and_test_fn(i, r);
    }
}

static inline BOOL_T refcount_dec_and_test(refcount_t *r)
{
    return refcount_sub_and_test(1, r);
}

static inline void refcount_inc(refcount_t *r)
{
    refcount_add(1, r);
}

static inline void refcount_dec(refcount_t *r)
{
    refcount_add(-1, r);
}

#ifdef __cplusplus
}
#endif
#endif 
