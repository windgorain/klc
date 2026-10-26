/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-12
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_NETNS_H_
#define _KLC_SYM_NETNS_H_

#ifdef __cplusplus
extern "C" {
#endif

struct net;


static inline void * get_init_net(void)
{
    static void *init_net = NULL;
    if (!init_net) { init_net = ulc_sys_get_sym("init_net"); }
    return init_net;
}


static inline struct net * klc_get_net_ns_by_pid(int pid)
{
    return (void*)(long)ulc_call_sym(0, klc_get_net_ns_by_pid, pid);
}


static inline void put_net(struct net *net)
{
    ulc_call_sym(0, put_net, net);
}


static inline struct net * klc_get_current_netns(void)
{
    return (void*)(long)ulc_call_sym(0, klc_get_current_netns);
}

#ifdef __cplusplus
}
#endif
#endif 
