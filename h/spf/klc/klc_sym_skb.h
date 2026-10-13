/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-12
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_SKB_H_
#define _KLC_SYM_SKB_H_

#ifdef __cplusplus
extern "C" {
#endif

struct sk_buff;

static inline int klc_get_skb_info(struct sk_buff *skb, OUT KLC_SKB_INFO_S *info)
{
    return ulc_call_sym(-1, klc_get_skb_info, skb, info);
}

static inline void klc_free_skb(void *skb)
{
    static void (*free_skb_func)(void *) = NULL;
    if (!free_skb_func) { free_skb_func = ulc_sys_get_sym("klc_free_skb"); }
    if (free_skb_func) { free_skb_func(skb); }
}

static inline BOOL_T pskb_may_pull(struct sk_buff *skb, unsigned int len)
{
    return ulc_call_sym(FALSE, klc_pskb_may_pull, skb, len);
}

static inline void * skb_put(struct sk_buff *skb, unsigned int len)
{
    return (void*)(long)ulc_call_sym(0, klc_skb_put, (long)skb, len);
}

#ifdef __cplusplus
}
#endif
#endif 
