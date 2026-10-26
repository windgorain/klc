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

enum {
    KLC_GFP_ATOMIC = 0,
    KLC_GFP_NOWAIT,
    KLC_GFP_KERNEL,

    KLC_GFP_MAX,
};

enum {
    GET_CPUS_TYPE_MAX = 0,  
    GET_CPUS_TYPE_POSSIBLE, 
    GET_CPUS_TYPE_PRESENT,  
    GET_CPUS_TYPE_ACTIVE,   
};

enum {
    WALK_CPUS_TYPE_POSSIBLE,
    WALK_CPUS_TYPE_PRESENT, 
    WALK_CPUS_TYPE_ONLINE,  
};

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
    unsigned short cb_off;
    unsigned short mark_off;
    unsigned short priority_off;
    unsigned short len_off;      
    unsigned short data_len_off; 
    unsigned short protocol_off;
    unsigned short sk_off;       
    unsigned short dev_off;
    unsigned short ip_summed_off;
    unsigned short csum_off;
    unsigned short head_off; 
    unsigned short end_off;  
    unsigned short data_off; 
    unsigned short tail_off; 
    unsigned short users_off;
}KLC_SKB_STRUCT_INFO_S;

typedef struct {
    unsigned char dev_addr[6]; 
    unsigned char reserved[2];
    unsigned int real_num_tx_queues; 
}KLC_NETDEV_INFO_S;

typedef struct {
    unsigned long long param[5];
}KLC_PT_PARAM_S;

typedef struct {
    void *kp __attribute__((aligned(8)));
    int kp_size __attribute__((aligned(8)));
    void *name __attribute__((aligned(8)));
    void *pre_handler __attribute__((aligned(8)));
    void *post_handler __attribute__((aligned(8)));
    void *fault_handler __attribute__((aligned(8)));
}KLC_KPROBE_PARAM_S;

typedef struct {
	int     (*ndo_init)(void *dev);
	void    (*ndo_uninit)(void *dev);
	int     (*ndo_open)(void *dev);
	int     (*ndo_stop)(void *dev);
	int     (*ndo_start_xmit)(void *skb, void *dev);
}KLC_NET_DEV_OPS_S;

#ifdef __cplusplus
}
#endif
#endif 
