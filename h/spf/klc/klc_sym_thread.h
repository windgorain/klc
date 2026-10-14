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

#ifdef __cplusplus
}
#endif
#endif 
