/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "klcko_impl.h"
#include "ko/ko_utl.h"
#include "ko/ko_netfilter.h"
#include "linux/bpf_verifier.h"
#include "../base/klcko_pt.h"
#include "utl/mybpf_tc_def.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,18,0)
#define KLCKO_OSBASE_BPF_OPS 
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0)
#define KLCKO_OSBASE_BPF_PROG
#endif

#ifdef BPF_MAX_SUBPROGS
#undef KLCKO_OSBASE_BPF_OPS 
#define KLCKO_OSBASE_BPF_OPS 
#undef KLCKO_OSBASE_BPF_PROG
#define KLCKO_OSBASE_BPF_PROG
#endif

#if 1 

int KlcKoFunc_NumPossibleCpus(void)
{
    return num_possible_cpus();
}

int KlcKoFunc_TimerInit(struct timer_list *timer, void *timeout_func)
{
    KO_SETUP_TIMER(timer, timeout_func, 0);
    return 0;
}

int klc_add_timer(struct timer_list *timer, unsigned int ms)
{
    unsigned int t;

    if (HZ < 1000) {
        t = round_up(ms, 1000/HZ);
        t = (t * HZ) / 1000;
    } else {
        t = (ms * HZ) / 1000;
    }

    timer->expires = jiffies + t;
    add_timer(timer);

    return 0;
}

int klc_del_timer(struct timer_list *timer)
{
    del_timer(timer);
    return 0;
}
#endif

int klc_init_nf_hook_ops(KLC_PARAM_S *p)
{
    struct nf_hook_ops *ops = (void*)(long)p->param[0];
    void *func = (void*)(long)p->param[1];
    u8 pf = p->param[2];
    u32 hooknum = p->param[3];
    int pri = p->param[4];

    KO_NF_FILL_OPS(ops, func, pf, hooknum, pri);

    return 0;
}

U64 klc_map_prog_run(struct bpf_map *progmap, unsigned int index, void *ctx)
{
    struct bpf_array *array = container_of(progmap, struct bpf_array, map);
    struct bpf_prog *prog;

    if (unlikely(index >= progmap->max_entries)) {
        return KLC_RET_ERR;
    }

    prog = READ_ONCE(array->ptrs[index]);
    if (! prog) {
        return KLC_RET_ERR;
    }

#ifndef BPF_PROG_RUN
#define BPF_PROG_RUN(filter, ctx)  (*(filter)->bpf_func)(ctx, (filter)->insnsi)
#endif

    return BPF_PROG_RUN(prog, ctx);
}

void * klc_get_bpf_env_prog(void *env)
{
#ifdef KLCKO_OSBASE_BPF_PROG
    struct bpf_verifier_env *tmp = env;
    return tmp->prog;
#else
    struct verifier_env {
        struct bpf_prog *prog;
    };
    struct verifier_env *tmp = env;
    return tmp->prog;
#endif
}

#ifndef KLCKO_OSBASE_BPF_OPS
static void * _klcko_getprog(void *env)
{
#ifdef KLCKO_OSBASE_BPF_PROG
    struct bpf_verifier_env *tmp = env;
    return tmp->prog;
#else
    struct verifier_env {
        struct bpf_prog *prog;
    };
    struct verifier_env *tmp = env;
    return tmp->prog;
#endif
}
#endif

void * klc_get_bpf_env_ops(void *env)
{
#ifdef KLCKO_OSBASE_BPF_OPS
    struct bpf_verifier_env *tmp = env;
    return &tmp->ops;
#else
    struct bpf_prog *prog = _klcko_getprog(env);
    return &prog->aux->ops;
#endif
}

int klc_set_bpf_len(struct bpf_prog *prog, int len )
{
	prog->len = len;
    return 0;
}

void * klc_set_bpf_func(struct bpf_prog *prog, void *f)
{
    void *old = prog->bpf_func;
    if (f) {
        prog->bpf_func = f;
    }
    return old;
}

int klc_get_bpf_len(struct bpf_prog *prog)
{
	return prog->len;
}
void * klc_get_bpf_insn(struct bpf_prog *prog)
{
    return prog->insnsi;
}
int klc_get_hz(void)
{
    return HZ;
}

int klc_capable_admin(void)
{
    return capable(CAP_SYS_ADMIN);
}

void * klc_get_path_private(struct path *path)
{
    struct dentry *d = path->dentry;
    struct inode *inode = d->d_inode;

    if ((! inode) || (IS_ERR(inode))){
        return NULL;
    }

    return inode->i_private;
}

void * klc_get_current_mm(void)
{
    return current->mm;
}

void * klc_get_current_netns(void)
{
	return current->nsproxy->net_ns;
}

int klc_get_bpf_proto_size(void)
{
    return sizeof(struct bpf_func_proto);
}

int klc_init_kprobe(struct kprobe *kp, int kp_size, KLC_KPROBE_PARAM_S *p)
{
    if (kp_size < sizeof(*kp)) {
        return -1;
    }

    memset(kp, 0, sizeof(*kp));

    kp->symbol_name = p->name;
    kp->pre_handler = p->pre_handler;
    kp->post_handler = p->post_handler;

    return 0;
}

void * klc_get_bpf_proto_func(struct bpf_verifier_ops *proto)
{
    return proto->get_func_proto;
}

int klc_set_bpf_proto_func(struct bpf_verifier_ops *proto, void *f)
{
    proto->get_func_proto = f;
    return 0;
}

int klc_get_bpf_verifier_ops_size(void)
{
    return sizeof(struct bpf_verifier_ops);
}

int klc_init_bpf_proto(struct bpf_func_proto *proto, void *func)
{
    if (proto->func) {
        return 0;
    }

    proto->func = func;
    proto->ret_type = RET_INTEGER;
    proto->arg1_type = ARG_ANYTHING;
    proto->arg2_type = ARG_ANYTHING;
    proto->arg3_type = ARG_ANYTHING;
    proto->arg4_type = ARG_ANYTHING;
    proto->arg5_type = ARG_ANYTHING;

    return 0;
}

void klcko_skb_reserve(struct sk_buff *skb, int len)
{
    skb_reserve(skb, len);
}

void klcko_skb_reset_network_header(struct sk_buff *skb)
{
    skb_reset_network_header(skb);
}

void klcko_skb_reset_transport_header(struct sk_buff *skb)
{
    skb_reset_transport_header(skb);
}

void klcko_skb_set_transport_header(struct sk_buff *skb, const int offset)
{
    skb_set_transport_header(skb, offset);
}

int klcko_skb_headroom(struct sk_buff *skb)
{
    return skb_headroom(skb);
}

void * klcko_create_skb(int data_len)
{
    struct sk_buff *skb;
    skb = alloc_skb(data_len + LL_MAX_HEADER, GFP_ATOMIC);
    if (! skb) {
        return NULL;
	}

    skb_reserve(skb, LL_MAX_HEADER);
    skb_reset_network_header(skb);

    return skb;
}

void klcko_free_skb(void *skb)
{
	kfree_skb(skb);
}

void klcko_get_skb_struct_info(OUT KLC_SKB_STRUCT_INFO_S *info)
{
    info->struct_size = sizeof(struct sk_buff);
    info->cb_offset = offsetof(struct sk_buff, cb);
    info->mark_offset = offsetof(struct sk_buff, mark);
    info->priority_offset  = offsetof(struct sk_buff, priority);
}

void klcko_get_skb_info(struct sk_buff *skb, OUT KLC_SKB_INFO_S *info)
{
    info->queue_mapping = skb->queue_mapping;
    info->protocol = skb->protocol;
    info->tc_index = skb->tc_index;
    info->vlan_tci = skb->vlan_tci;
    info->vlan_proto = skb->vlan_proto;
    info->len= skb->len;
    info->fragment_len= skb->data_len;
    info->head_len = skb->len - skb->data_len;
    info->pkt_type = skb->pkt_type;
    info->tc_at_ingress = skb->tc_at_ingress;
    info->ingress_ifindex = skb->skb_iif;
    info->ifindex = skb->dev->ifindex;
    info->mark = skb->mark;
    info->hash = skb->hash;
    info->napi_id = skb->napi_id;
    info->priority = skb->priority;
    info->data = (u64)(unsigned long)skb->data;
    info->head = (u64)(unsigned long)skb->head;
    info->end = (u64)(unsigned long)skb->end;
}

void klcko_compute_data_pointers(struct sk_buff *skb, MYBPF_TC_S *tc)
{
    bpf_compute_data_pointers(skb);
    if (tc) {
        tc->data = skb->data;
        tc->data_end = skb->data + skb_headlen(skb);
        tc->data_meta = skb->data - skb_metadata_len(skb);
    }
}

unsigned int klcko_xdp_ingress_ifindex(struct xdp_buff *x)
{
    return x->rxq->dev->ifindex;
}

unsigned int klcko_xdp_rx_queue_index(struct xdp_buff *x)
{
    return x->rxq->queue_index;
}

long klcko_sprintf(char *str, U32 str_size, const char *fmt, U64 *d, U32 d_len)
{
    switch (d_len) {
        case 0: return snprintf(str,str_size,"%s",fmt);
        case 8: return snprintf(str,str_size,fmt,d[0]);
        case 16: return snprintf(str,str_size,fmt,d[0],d[1]);
        case 24: return snprintf(str,str_size,fmt,d[0],d[1],d[2]);
        case 32: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3]);
        case 40: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4]);
        case 48: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5]);
        case 56: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6]);
        case 64: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7]);
        case 72: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8]);
        case 80: return snprintf(str,str_size,fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9]);
        default: return -1;
    }
}

int klc_get_pt_params(struct pt_regs *regs, OUT KLC_PT_PARAM_S *p)
{
    p->param[0] = PT_REGS_PARM1(regs);
    p->param[1] = PT_REGS_PARM2(regs);
    p->param[2] = PT_REGS_PARM3(regs);
    p->param[3] = PT_REGS_PARM4(regs);
    p->param[4] = PT_REGS_PARM5(regs);
    return 0;
}
void klcko_set_pt_rc(struct pt_regs *regs, long long rc)
{
    PT_REGS_RC(regs) = rc;
}

void * klcko_alloc_percpu_mem(int percpu_size, int align)
{
    return __alloc_percpu_gfp(percpu_size, align, GFP_ATOMIC);
}

void klcko_free_percpu_mem(void *ptr)
{
    free_percpu(ptr);
}

int KlcKoFunc_Init(void)
{
    return 0;
}

