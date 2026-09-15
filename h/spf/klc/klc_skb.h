/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-15
* Description: 
******************************************************************************/
#ifndef _KLC_SKB_H_
#define _KLC_SKB_H_

#ifdef __cplusplus
extern "C" {
#endif

static inline void klc_free_skb(void *skb)
{
    ulc_call_sym(0, klc_free_skb, skb);
}

#ifdef __cplusplus
}
#endif
#endif 
