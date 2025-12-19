/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _KLC_DEF_H_
#define _KLC_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define KLC_KO_VERSION 1
#define KLC_NAME_SIZE 32
#define KLC_FUNC_NAME_SIZE KLC_NAME_SIZE


#define KLC_RET_ERR ((U64)(-1LL))

#define KLC_PARAM_NUM_MAX 5
typedef struct {
    unsigned char count;
    unsigned long long param[KLC_PARAM_NUM_MAX];
}KLC_PARAM_S;

typedef struct {
    unsigned short queue_mapping;
    unsigned short protocol;
    unsigned short tc_index;
    unsigned short vlan_tci;
    unsigned short vlan_proto;
    unsigned short len; 
    unsigned short fragment_len; 
    unsigned short head_len; 

    unsigned int pkt_type:3;
    unsigned int tc_at_ingress:1;
    unsigned int ingress_ifindex;
    unsigned int ifindex;
    unsigned int mark;
    unsigned int hash;
    unsigned int napi_id;
    unsigned int priority;

    unsigned long long data;
    unsigned long long head;
    unsigned long long end;
}KLC_SKB_INFO_S;

typedef struct {
    unsigned short struct_size; 
    unsigned short cb_offset;
    unsigned short mark_offset;
    unsigned short priority_offset;
}KLC_SKB_STRUCT_INFO_S;

typedef struct {
    unsigned long long param[5];
}KLC_PT_PARAM_S;

typedef struct {
    void *name;
    void *pre_handler;
    void *post_handler;
    void *fault_handler;
}KLC_KPROBE_PARAM_S;

#ifdef __cplusplus
}
#endif
#endif 
