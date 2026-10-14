/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-12
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_PID_H_
#define _KLC_SYM_PID_H_

#ifdef __cplusplus
extern "C" {
#endif

struct pid;
struct task_struct;


static inline struct pid * find_vpid(int pid)
{
    return (void*)(long)ulc_call_sym(0, find_vpid, pid);
}


static inline struct task_struct * pid_task(struct pid *pid, int type)
{
    U64 p1 = (long)pid;

    if (klc_is_sys32()) {
        p1 |= (((U64)type) << 32);
    }

    return (void*)(long)ulc_call_sym(0, pid_task, p1, type);
}


static inline struct pid * find_get_pid(int pid)
{
    return (void*)(long)ulc_call_sym(0, find_get_pid, pid);
}

static inline void put_pid(struct pid *pid)
{
    ulc_call_sym(0, put_pid, pid);
}


static inline struct task_struct * get_pid_task(struct pid *pid, int type)
{
    U64 p1 = (long)pid;

    if (klc_is_sys32()) {
        p1 |= (((U64)type) << 32);
    }

    return (void*)(long)ulc_call_sym(0, get_pid_task, p1, type);
}


static inline void put_task_struct(struct task_struct *t)
{
    ulc_call_sym(0, klc_put_task_struct, t);
}

#ifdef __cplusplus
}
#endif
#endif 
