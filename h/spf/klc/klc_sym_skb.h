/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-12
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_SKB_H_
#define _KLC_SYM_SKB_H_

#include "spf/spf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PACKET_HOST
#define PACKET_HOST		    0		
#define PACKET_BROADCAST	1		
#define PACKET_MULTICAST	2		
#define PACKET_OTHERHOST	3		
#define PACKET_OUTGOING		4		
#define PACKET_LOOPBACK		5		
#define PACKET_USER		    6		
#define PACKET_KERNEL		7		
#define PACKET_FASTROUTE	6		
#endif

#ifndef CHECKSUM_NONE
#define CHECKSUM_NONE		0
#define CHECKSUM_UNNECESSARY	1
#define CHECKSUM_COMPLETE	2
#define CHECKSUM_PARTIAL	3
#endif

#define LL_MAX_HEADER   128
#define NET_SKB_PAD	    32

struct sk_buff;


static inline void skb_reserve(struct sk_buff *skb, int len)
{
    static void (*skb_reserve_fn)(void *, int) = NULL;
    if (! skb_reserve_fn) { 
        skb_reserve_fn = ulc_sys_get_sym("skb_reserve");
        if (! skb_reserve_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)len) << 32);
        void (*skb_reserve_fn32)(U64) = (void*)skb_reserve_fn;
        skb_reserve_fn32(p1);
    } else {
        skb_reserve_fn(skb, len);
    }
}


static inline struct sk_buff * alloc_skb(unsigned int size, unsigned int priority)
{
    static void * (*alloc_skb_fn)(unsigned int, unsigned int) = NULL;
    if (! alloc_skb_fn) {
        alloc_skb_fn = ulc_sys_get_sym("alloc_skb");
        if (! alloc_skb_fn) {
            return NULL;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = size; p1 |= (((U64)priority) << 32);
        void * (*alloc_skb_fn32)(U64) = (void*)alloc_skb_fn;
        return alloc_skb_fn32(p1);
    } else {
        return alloc_skb_fn(size, priority);
    }
}


static inline struct sk_buff * klc_alloc_skb(unsigned int size, U32 klc_gfp_flag)
{
    return alloc_skb(size, klc_gfp_flags(klc_gfp_flag));
}


static inline struct sk_buff * klc_create_skb(unsigned int size, U32 klc_gfp_flag)
{
    struct sk_buff *skb = klc_alloc_skb(size + LL_MAX_HEADER, klc_gfp_flag);
    if (! skb) {
        return NULL;
    }

    skb_reserve(skb, LL_MAX_HEADER);

    return skb;
}

static inline void kfree_skb(void *skb)
{
    static void (*free_skb_fn)(void *) = NULL;
    if (!free_skb_fn) {
        free_skb_fn = ulc_sys_get_sym("kfree_skb");
        if (! free_skb_fn ) {
            return;
        }
    }

    free_skb_fn(skb);
}

static inline int klc_get_skb_info(struct sk_buff *skb, OUT KLC_SKB_INFO_S *info)
{
    static int (*get_skb_info_fn)(void *skb, void *info) = NULL;
    if (! get_skb_info_fn) {
        get_skb_info_fn = ulc_sys_get_sym("klc_get_skb_info");
        if (! get_skb_info_fn) {
            return -1;
        }
    }

    return get_skb_info_fn(skb, info);
}

static inline KLC_SKB_STRUCT_INFO_S * klc_get_skb_struct_info(void)
{
    static KLC_SKB_STRUCT_INFO_S skb_struct_info = {0};
    static int (*get_skb_struct_info_fn)(void *info) = NULL;

    if (skb_struct_info.struct_size == 0) {
        if (! get_skb_struct_info_fn) {
            get_skb_struct_info_fn = ulc_sys_get_sym("klc_get_skb_struct_info");
            if (! get_skb_struct_info_fn) {
                return NULL;
            }
        }
        get_skb_struct_info_fn(&skb_struct_info);
    }

    return &skb_struct_info;
}


static inline int klc_skb_len(struct sk_buff *skb)
{
    KLC_SKB_STRUCT_INFO_S *info = klc_get_skb_struct_info();
    if (! info) {
        return 0;
    }

    unsigned int *ptr = (void*)skb + info->len_off;

    return *ptr;
}


static inline int klc_skb_data_len(struct sk_buff *skb)
{
    KLC_SKB_STRUCT_INFO_S *info = klc_get_skb_struct_info();
    if (! info) {
        return 0;
    }

    unsigned int *ptr = (void*)skb + info->data_len_off;

    return *ptr;
}


static inline int klc_skb_head_len(struct sk_buff *skb)
{
    int len = klc_skb_len(skb);
    int data_len = klc_skb_data_len(skb);
    return len - data_len;
}

static inline void * klc_skb_data(struct sk_buff *skb)
{
    KLC_SKB_STRUCT_INFO_S *info = klc_get_skb_struct_info();
    if (! info) {
        return NULL;
    }

    void **ptr = (void*)skb + info->data_off;

    if (klc_is_sys32()) {
        return (void*)(long)(*(unsigned int*)ptr);
    } else {
        return *ptr;
    }
}

static inline void * skb_tail_pointer(struct sk_buff *skb)
{
    static void * (*skb_tail_pointer_fn)(void *) = NULL;
    if (!skb_tail_pointer_fn) {
        skb_tail_pointer_fn = ulc_sys_get_sym("skb_tail_pointer");
        if (! skb_tail_pointer_fn) {
            return NULL;
        }
    }

    return skb_tail_pointer_fn(skb);
}

static inline void klc_skb_set_dev(struct sk_buff *skb, void *dev )
{
    KLC_SKB_STRUCT_INFO_S *info = klc_get_skb_struct_info();
    if (! info) {
        return;
    }

    void **ptr = (void*)skb + info->dev_off;

    if (klc_is_sys32()) {
        *(int*)ptr = (int)(long)dev;
    } else {
        *ptr = dev;
    }
}

static inline void klc_skb_set_protocol(struct sk_buff *skb, U16 protocol)
{
    KLC_SKB_STRUCT_INFO_S *info = klc_get_skb_struct_info();
    if (! info) {
        return;
    }

    U16 *ptr = (void*)skb + info->protocol_off;
    *ptr = protocol;
}

static inline U16 klc_skb_protocol(struct sk_buff *skb)
{
    KLC_SKB_STRUCT_INFO_S *info = klc_get_skb_struct_info();
    if (! info) {
        return 0;
    }

    U16 *ptr = (void*)skb + info->protocol_off;
    return *ptr;
}


static inline int klc_skb_add_users(struct sk_buff *skb, int val)
{
    KLC_SKB_STRUCT_INFO_S *info = klc_get_skb_struct_info();
    if (! info) {
        return -1;
    }

    void *ptr = (void*)skb + info->users_off;

    refcount_add(val, ptr);

    return 0;
}

static inline U8 klc_skb_get_ip_summed(struct sk_buff *skb)
{
    static U8 (*klc_skb_get_ip_summed_fn)(void *) = NULL;
    if (!klc_skb_get_ip_summed_fn) {
        klc_skb_get_ip_summed_fn = ulc_sys_get_sym("klc_skb_get_ip_summed");
        if (! klc_skb_get_ip_summed_fn) {
            return 0;
        }
    }

    return klc_skb_get_ip_summed_fn(skb);
}

static inline void klc_skb_set_ip_summed(struct sk_buff *skb, U8 ip_summed)
{
    static void * (*klc_skb_set_ip_summed_fn)(void *, U8) = NULL;
    if (!klc_skb_set_ip_summed_fn) {
        klc_skb_set_ip_summed_fn = ulc_sys_get_sym("klc_skb_set_ip_summed");
        if (! klc_skb_set_ip_summed_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)ip_summed) << 32);
        void (*klc_skb_set_ip_summed_fn32)(U64) = (void*)klc_skb_set_ip_summed_fn;
        return klc_skb_set_ip_summed_fn32(p1);
    } else {
        klc_skb_set_ip_summed_fn(skb, ip_summed);
    }
}

static inline U8 klc_skb_get_pkt_type(struct sk_buff *skb)
{
    static U8 (*klc_skb_get_pkt_type_fn)(void *) = NULL;
    if (!klc_skb_get_pkt_type_fn) {
        klc_skb_get_pkt_type_fn = ulc_sys_get_sym("klc_skb_get_pkt_type");
        if (! klc_skb_get_pkt_type_fn) {
            return 0;
        }
    }

    return klc_skb_get_pkt_type_fn(skb);
}

static inline void klc_skb_set_pkt_type(struct sk_buff *skb, U8 pkt_type)
{
    static void * (*klc_skb_set_pkt_type_fn)(void *, U8) = NULL;
    if (!klc_skb_set_pkt_type_fn) {
        klc_skb_set_pkt_type_fn = ulc_sys_get_sym("klc_skb_set_pkt_type");
        if (! klc_skb_set_pkt_type_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)pkt_type) << 32);
        void (*klc_skb_set_pkt_type_fn32)(U64) = (void*)klc_skb_set_pkt_type_fn;
        return klc_skb_set_pkt_type_fn32(p1);
    } else {
        klc_skb_set_pkt_type_fn(skb, pkt_type);
    }
}


static inline BOOL_T pskb_may_pull(struct sk_buff *skb, unsigned int len)
{
    static BOOL_T (*pskb_may_pull_fn)(void *, unsigned int) = NULL;
    if (! pskb_may_pull_fn) {
        pskb_may_pull_fn = ulc_sys_get_sym("pskb_may_pull");
        if (! pskb_may_pull_fn) {
            return FALSE;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)len) << 32);
        BOOL_T (*pskb_may_pull_fn32)(U64) = (void*)pskb_may_pull_fn;
        return pskb_may_pull_fn32(p1);
    } else {
        return pskb_may_pull_fn(skb, len);
    }
}



static inline void * skb_put(struct sk_buff *skb, unsigned int len)
{
    static void * (*skb_put_fn)(void *, unsigned int) = NULL;
    if (! skb_put_fn) {
        skb_put_fn = ulc_sys_get_sym("skb_put");
        if (! skb_put_fn) {
            return NULL;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)len) << 32);
        void * (*skb_put_fn32)(U64) = (void*)skb_put_fn;
        return skb_put_fn32(p1);
    } else {
        return skb_put_fn(skb, len);
    }
}


static inline void * skb_put_zero(struct sk_buff *skb, unsigned int len)
{
    void *tmp = skb_put(skb, len);
    if (tmp) {
        memset(tmp, 0, len);
    }
    return tmp;
}


static inline void *skb_put_data(struct sk_buff *skb, const void *data, unsigned int len)
{
	void *tmp = skb_put(skb, len);
    if (tmp) {
	    memcpy(tmp, data, len);
    }
	return tmp;
}


static inline void * skb_pull(struct sk_buff *skb, unsigned int len)
{
    static void * (*skb_pull_fn)(void *, unsigned int) = NULL;
    if (! skb_pull_fn) {
        skb_pull_fn = ulc_sys_get_sym("skb_pull");
        if (! skb_pull_fn) {
            return NULL;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)len) << 32);
        void * (*skb_pull_fn32)(U64) = (void*)skb_pull_fn;
        return skb_pull_fn32(p1);
    } else {
        return skb_pull_fn(skb, len);
    }
}


static inline void * skb_push(struct sk_buff *skb, unsigned int len)
{
    static void * (*skb_push_fn)(struct sk_buff *skb, unsigned int len) = NULL;

    if (! skb_push_fn) {
        skb_push_fn = ulc_sys_get_sym("skb_push");
        if (! skb_push_fn) {
            return NULL;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)len) << 32);
        void * (*skb_push_fn32)(U64) = (void*)skb_push_fn;
        return skb_push_fn32(p1);
    } else {
        return skb_push_fn(skb, len);
    }
}


static inline void skb_reset_network_header(struct sk_buff *skb)
{
    static void (*skb_reset_network_header_fn)(void *) = NULL;
    if (! skb_reset_network_header_fn) { 
        skb_reset_network_header_fn = ulc_sys_get_sym("skb_reset_network_header");
        if (! skb_reset_network_header_fn) {
            return;
        }
    }

    return skb_reset_network_header_fn(skb);
}


static inline void skb_set_network_header(struct sk_buff *skb, const int offset)
{
    static void (*skb_set_network_header_fn)(void *, const int) = NULL;
    if (! skb_set_network_header_fn) { 
        skb_set_network_header_fn = ulc_sys_get_sym("skb_set_network_header");
        if (! skb_set_network_header_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)(U32)offset) << 32);
        void (*skb_set_network_header_fn32)(U64) = (void*)skb_set_network_header_fn;
        skb_set_network_header_fn32(p1);
    } else {
        skb_set_network_header_fn(skb, offset);
    }
}


static inline void skb_reset_transport_header(struct sk_buff *skb)
{
    static void (*skb_reset_transport_header_fn)(void *) = NULL;
    if (! skb_reset_transport_header_fn) { 
        skb_reset_transport_header_fn = ulc_sys_get_sym("skb_reset_transport_header");
        if (! skb_reset_transport_header_fn) {
            return;
        }
    }

    return skb_reset_transport_header_fn(skb);
}


static inline void skb_set_transport_header(struct sk_buff *skb, const int offset)
{
    static void (*skb_set_transport_header_fn)(void *, const int) = NULL;
    if (! skb_set_transport_header_fn) { 
        skb_set_transport_header_fn = ulc_sys_get_sym("skb_set_transport_header");
        if (! skb_set_transport_header_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)(U32)offset) << 32);
        void (*skb_set_transport_header_fn32)(U64) = (void*)skb_set_transport_header_fn;
        skb_set_transport_header_fn32(p1);
    } else {
        skb_set_transport_header_fn(skb, offset);
    }
}


static inline void skb_reset_mac_header(struct sk_buff *skb)
{
    static void (*skb_reset_mac_header_fn)(void *) = NULL;
    if (! skb_reset_mac_header_fn) { 
        skb_reset_mac_header_fn = ulc_sys_get_sym("skb_reset_mac_header");
        if (! skb_reset_mac_header_fn) {
            return;
        }
    }

    return skb_reset_mac_header_fn(skb);
}


static inline void skb_set_mac_header(struct sk_buff *skb, const int offset)
{
    static void (*skb_set_mac_header_fn)(void *, const int) = NULL;
    if (! skb_set_mac_header_fn) { 
        skb_set_mac_header_fn = ulc_sys_get_sym("skb_set_mac_header");
        if (! skb_set_mac_header_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)(U32)offset) << 32);
        void (*skb_set_mac_header_fn32)(U64) = (void*)skb_set_mac_header_fn;
        skb_set_mac_header_fn32(p1);
    } else {
        skb_set_mac_header_fn(skb, offset);
    }
}


static inline void skb_set_queue_mapping(struct sk_buff *skb, U16 queue_mapping)
{
    static void (*skb_set_queue_mapping_fn)(void *, U16) = NULL;
    if (! skb_set_queue_mapping_fn) { 
        skb_set_queue_mapping_fn = ulc_sys_get_sym("skb_set_queue_mapping");
        if (! skb_set_queue_mapping_fn) {
            return;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)queue_mapping) << 32);
        void (*skb_set_queue_mapping_fn32)(U64) = (void*)skb_set_queue_mapping_fn;
        skb_set_queue_mapping_fn32(p1);
    } else {
        skb_set_queue_mapping_fn(skb, queue_mapping);
    }
}

static inline struct sk_buff * skb_clone(struct sk_buff *skb, unsigned int gfp_flag)
{
    static void * (*skb_clone_fn)(void *, U16) = NULL;
    if (! skb_clone_fn) { 
        skb_clone_fn = ulc_sys_get_sym("skb_clone");
        if (! skb_clone_fn) {
            return NULL;
        }
    }

    if (klc_is_sys32()) {
        U64 p1 = (U32)(long)skb; p1 |= (((U64)gfp_flag) << 32);
        void * (*skb_clone_fn32)(U64) = (void*)skb_clone_fn;
        return skb_clone_fn32(p1);
    } else {
        return skb_clone_fn(skb, gfp_flag);
    }
}

#ifdef __cplusplus
}
#endif
#endif 
