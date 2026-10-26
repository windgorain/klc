/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-13
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_THREAD_H_
#define _KLC_SYM_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TASK_COMM_LEN 16

struct task_struct;


static inline struct task_struct * kthread_create(void *fn, void *data, const char *name)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)fn; p1 |= (((U64)(U32)(long)data) << 32);
        U64 p2 = 0xffffffff; p2 |= (((U64)(U32)(long)name) << 32);
        return (void*)(long)ulc_call_sym(0, kthread_create_on_node, p1, p2);
    } else {
        return (void*)(long)ulc_call_sym(0, kthread_create_on_node, fn, data, -1, name);
    }
}



static inline struct task_struct * kthread_create_on_cpu(void *fn, void *data, U32 cpu, const char *namefmt)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)fn; p1 |= (((U64)(U32)(long)data) << 32);
        U64 p2 = cpu; p2 |= (((U64)(U32)(long)namefmt) << 32);
        return (void*)(long)ulc_call_sym(0, kthread_create_on_cpu, p1, p2);
    } else {
        return (void*)(long)ulc_call_sym(0, kthread_create_on_cpu, fn, data, cpu, namefmt);
    }
}


static inline int wake_up_process(struct task_struct *tsk)
{
    return ulc_call_sym(0, wake_up_process, tsk);
}


static inline void set_task_comm(struct task_struct *tsk, char *name)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)tsk; p1 |= (((U64)(U32)(long)name) << 32);
        ulc_call_sym(0, __set_task_comm, p1, 0);
    } else {
        ulc_call_sym(0, __set_task_comm, tsk, name, 0);
    }
}

static inline BOOL_T kthread_should_stop(void)
{
    static BOOL_T (*kthread_should_stop_fn)(void) = NULL;
    if (! kthread_should_stop_fn) {
        kthread_should_stop_fn = ulc_sys_get_sym("kthread_should_stop");
        if (! kthread_should_stop_fn) {
            return TRUE;
        }
    }

    return kthread_should_stop_fn();
}

static inline int kthread_stop(struct task_struct *k)
{
    static int (*kthread_stop_fn)(void *) = NULL;
    if (! kthread_stop_fn) {
        kthread_stop_fn = ulc_sys_get_sym("kthread_stop");
        if (! kthread_stop_fn) {
            return 0;
        }
    }

    return kthread_stop_fn(k);
}
#ifdef __cplusplus
}
#endif
#endif 
