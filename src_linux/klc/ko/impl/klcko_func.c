/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "klcko_impl.h"
#include "ko/ko_utl.h"
#include "ko/ko_netfilter.h"
#include "linux/bpf_verifier.h"
#include "linux/freezer.h"
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

U32 klc_gfp_flags(int type)
{
    switch (type) {
        case KLC_GFP_ATOMIC: return GFP_ATOMIC;
        case KLC_GFP_NOWAIT: return GFP_NOWAIT;
        case KLC_GFP_KERNEL: return GFP_KERNEL;
    }

    return 0;
}

int klc_num_cpus(int type)
{
    switch (type) {
        case GET_CPUS_TYPE_MAX: return nr_cpu_ids;
        case GET_CPUS_TYPE_POSSIBLE: return num_possible_cpus();
        case GET_CPUS_TYPE_PRESENT: return num_present_cpus();
        case GET_CPUS_TYPE_ACTIVE: return num_active_cpus();
    }

    return 0;
}

int klc_walk_cpus(void (*func)(int), int type)
{
    int cpu;

    switch (type) {
        case WALK_CPUS_TYPE_POSSIBLE: for_each_possible_cpu(cpu) { func(cpu); } break;
        case WALK_CPUS_TYPE_ONLINE: for_each_online_cpu(cpu) { func(cpu); } break;
        case WALK_CPUS_TYPE_PRESENT: for_each_present_cpu(cpu) { func(cpu); } break;
    }

    return 0;
}

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

U64 klc_map_prog_run(U64 p1, U64 p2, U64 p3)
{
    struct bpf_map *progmap = (void*)(LONG)p1;
    unsigned int index = p2;
    void *ctx = (void*)(LONG)p3;

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

int klc_set_bpf_len(U64 p1, U64 p2)
{
    struct bpf_prog *prog = (void*)(LONG)p1;
    int len = p2; 

	prog->len = len;
    return 0;
}


void * klc_set_bpf_func(U64 p1, U64 p2)
{
    struct bpf_prog *prog = (void*)(LONG)p1;
    void *f = (void*)(LONG)p2;

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

int klc_register_kprobe(KLC_KPROBE_PARAM_S *p)
{
    struct kprobe *kp = p->kp;

    if (p->kp_size < sizeof(*kp)) {
        return -1;
    }

    memset(kp, 0, sizeof(*kp));

    kp->symbol_name = p->name;
    kp->pre_handler = p->pre_handler;
    kp->post_handler = p->post_handler;

    return register_kprobe(kp);
}

void * klc_get_bpf_proto_func(struct bpf_verifier_ops *proto)
{
    return proto->get_func_proto;
}

int klc_set_bpf_proto_func(U64 p1, U64 p2)
{
    struct bpf_verifier_ops *proto = (void*)(long)p1;
    proto->get_func_proto = (void*)(long)p2;
    return 0;
}

int klc_get_bpf_verifier_ops_size(void)
{
    return sizeof(struct bpf_verifier_ops);
}

int klc_init_bpf_proto(U64 p1, U64 p2)
{
    struct bpf_func_proto *proto = (void*)(LONG)p1;
    void *func = (void*)(LONG)p2;

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

void klc_get_skb_struct_info(OUT KLC_SKB_STRUCT_INFO_S *info)
{
    info->struct_size = sizeof(struct sk_buff);
    info->cb_off = offsetof(struct sk_buff, cb);
    info->mark_off = offsetof(struct sk_buff, mark);
    info->priority_off = offsetof(struct sk_buff, priority);
    info->len_off = offsetof(struct sk_buff, len);
    info->data_len_off = offsetof(struct sk_buff, data_len);
    info->protocol_off = offsetof(struct sk_buff, protocol);
    info->sk_off = offsetof(struct sk_buff, sk);
    info->dev_off = offsetof(struct sk_buff, dev);
    info->csum_off = offsetof(struct sk_buff, csum);
    info->head_off = offsetof(struct sk_buff, head);
    info->end_off = offsetof(struct sk_buff, end);
    info->data_off = offsetof(struct sk_buff, data);
    info->tail_off = offsetof(struct sk_buff, tail);
    info->users_off = offsetof(struct sk_buff, users);
}

int klc_get_skb_info(U64 p1, U64 p2)
{
    struct sk_buff *skb = (void*)(LONG)p1;
    OUT KLC_SKB_INFO_S *info = (void*)(LONG)p2;

    info->queue_mapping = skb->queue_mapping;
    info->protocol = skb->protocol;
    info->tc_index = skb->tc_index;
    info->vlan_tci = skb->vlan_tci;
    info->vlan_proto = skb->vlan_proto;
    info->len = skb->len;
    info->fragment_len = skb->data_len;
    info->head_len = skb->len - skb->data_len;
    info->pkt_type = skb->pkt_type;
    info->tc_at_ingress = skb->tc_at_ingress;
    info->ingress_ifindex = skb->skb_iif;
    info->ifindex = -1;
    if (skb->dev) { info->ifindex = skb->dev->ifindex; }
    info->mark = skb->mark;
    info->hash = skb->hash;
    info->napi_id = skb->napi_id;
    info->priority = skb->priority;
    info->data = (u64)(unsigned long)skb->data;
    info->head = (u64)(unsigned long)skb->head;
    info->end = (u64)(unsigned long)skb->end;

    return 0;
}

void klc_skb_set_pkt_type(U64 p1, U64 p2)
{
    struct sk_buff *skb = (void*)(LONG)p1;
    skb->pkt_type = p2;
}

U8 klc_skb_get_pkt_type(struct sk_buff *skb)
{
    return skb->pkt_type;
}

void klc_skb_set_ip_summed(U64 p1, U64 p2)
{
    struct sk_buff *skb = (void*)(LONG)p1;
    skb->ip_summed = p2;
}

U8 klc_skb_get_ip_summed(struct sk_buff *skb)
{
    return skb->ip_summed;
}


void klc_compute_data_pointers(U64 p1, U64 p2)
{
    struct sk_buff *skb = (void*)(LONG)p1;
    MYBPF_TC_S *tc = (void*)(LONG)p2;

    bpf_compute_data_pointers(skb);
    if (tc) {
        tc->data = skb->data;
        tc->data_end = skb->data + skb_headlen(skb);
        tc->data_meta = skb->data - skb_metadata_len(skb);
    }
}

unsigned int klc_xdp_ingress_ifindex(struct xdp_buff *x)
{
    return x->rxq->dev->ifindex;
}

unsigned int klc_xdp_rx_queue_index(struct xdp_buff *x)
{
    return x->rxq->queue_index;
}

long klc_sprintf(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5)
{
    char *str = (void*)(LONG)p1;
    U32 str_size = p2;
    const char *fmt = (void*)(LONG)p3;
    U64 *d = (void*)(LONG)p4;
    U32 d_len = p5;

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

int klc_get_pt_params(U64 p1, U64 p2)
{
    struct pt_regs *regs = (void*)(LONG)p1;
    KLC_PT_PARAM_S *p = (void*)(LONG)p2;

    p->param[0] = PT_REGS_PARM1(regs);
    p->param[1] = PT_REGS_PARM2(regs);
    p->param[2] = PT_REGS_PARM3(regs);
    p->param[3] = PT_REGS_PARM4(regs);
    p->param[4] = PT_REGS_PARM5(regs);
    return 0;
}

void klc_set_pt_rc(U64 p1, U64 p2)
{
    struct pt_regs *regs = (void*)(LONG)p1;
    PT_REGS_RC(regs) = p2;
}

void __percpu * klc_alloc_percpu_mem(U64 p1, U64 p2)
{
    int percpu_size = p1;
    int align = p2;

    return __alloc_percpu_gfp(percpu_size, align, GFP_ATOMIC);
}

void klc_free_percpu_mem(void __percpu *ptr)
{
    free_percpu(ptr);
}

int klc_init_netdev(U64 p1, U64 p2, U64 p3, U64 p4)
{
    struct net_device *dev = (void*)(LONG)p1;
    char *name = (void*)(LONG)p2;
    KLC_NET_DEV_OPS_S *ops = (void*)(LONG)p3;
    OUT struct net_device_ops *netdev_ops = (void*)(LONG)p4;

    
    strscpy(dev->name, name, sizeof(dev->name));
    netdev_ops->ndo_init = (void*)ops->ndo_init;
    netdev_ops->ndo_uninit = (void*)ops->ndo_uninit;
    netdev_ops->ndo_open = (void*)ops->ndo_open;
    netdev_ops->ndo_stop = (void*)ops->ndo_stop;
    netdev_ops->ndo_start_xmit = (void*)ops->ndo_start_xmit;
    dev->netdev_ops = netdev_ops;

    return 0;
}

int klc_netdev_get_info(U64 p1, U64 p2)
{
    struct net_device *dev = (void*)(long)p1;
    OUT KLC_NETDEV_INFO_S *info = (void*)(long)p2;

    info->real_num_tx_queues = dev->real_num_tx_queues;
    ether_addr_copy(info->dev_addr, dev->dev_addr);

    return 0;
}

void klc_put_task_struct(void *t)
{
    put_task_struct(t);
}


struct net * klc_get_net_ns_by_pid(pid_t pid)
{
    struct task_struct *tsk;
    struct net *net = NULL;
    
    rcu_read_lock();
	tsk = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (tsk) {
        task_lock(tsk);
        if (tsk->nsproxy)
            net = get_net(tsk->nsproxy->net_ns);
        task_unlock(tsk);
    }
    rcu_read_unlock();
    
    return net;
}


struct sock * klc_get_sock_by_socket(struct socket *res)
{
    return res->sk;
}

int klc_socket_recv(struct socket *sock, void *buf, int buf_len, int flags)
{
    struct msghdr msg = {0};
    struct kvec iov;

    
    iov.iov_base = buf;
    iov.iov_len  = buf_len;

    
#ifdef ITER_DEST
    iov_iter_kvec(&msg.msg_iter, ITER_DEST, &iov, 1, buf_len);
#else
    iov_iter_kvec(&msg.msg_iter, READ, &iov, 1, buf_len);
#endif

    return sock_recvmsg(sock, &msg, flags);
}

int klc_socket_send(struct socket *sock, void *buf, int buf_len, int flags)
{
    struct msghdr msg = {0};
    struct kvec iov;

    iov.iov_base = buf;
    iov.iov_len  = buf_len;
    msg.msg_flags = flags;

#ifdef ITER_SOURCE
    iov_iter_kvec(&msg.msg_iter, ITER_SOURCE, &iov, 1, buf_len);
#else
    iov_iter_kvec(&msg.msg_iter, WRITE, &iov, 1, buf_len);
#endif

    return sock_sendmsg(sock, &msg);
}

int klc_sock_setsockopt(struct socket *sock, int level, int optname, void *optval, int optlen)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 7, 0)
    return do_sock_setsockopt(sock, false, level, optname, KERNEL_SOCKPTR(optval), optlen);
#elif defined(KERNEL_SOCKPTR)
    int (*setter)(struct socket *, int, int, sockptr_t, unsigned int) = (level == SOL_SOCKET ? sock_setsockopt : sock->ops->setsockopt);
	return setter(sock, level, optname, KERNEL_SOCKPTR(optval), (U32)optlen);
#else
    return -1;
#endif
}

bool klc_need_resched(void)
{
    return need_resched();
}

bool klc_try_to_freeze(void)
{
    return try_to_freeze();
}

void klc_hard_tx_lock(U64 p1, U64 p2)
{
    struct net_device *dev = (void*)(long)p1;
    void *txq = (void*)(long)p2;
    HARD_TX_LOCK(dev, txq, smp_processor_id());
}

void klc_hard_tx_unlock(U64 p1, U64 p2)
{
    struct net_device *dev = (void*)(long)p1;
    void *txq = (void*)(long)p2;
    HARD_TX_UNLOCK(dev, txq);
}
int KlcKoFunc_Init(void)
{
    return 0;
}

