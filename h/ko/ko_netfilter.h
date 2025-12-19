/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _KO_NETFILTER_H
#define _KO_NETFILTER_H
#ifdef __cplusplus
extern "C"
{
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
#define KO_NF_HOOK_PARAMS void *priv, struct sk_buff *skb, const struct nf_hook_state *state
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3,0,0)
#ifndef __GENKSYMS__
#define KO_NF_HOOK_PARAMS const struct nf_hook_ops *ops, struct sk_buff *skb, \
    const struct net_device *in, const struct net_device *out, const struct nf_hook_state *state
#else
#define KO_NF_HOOK_PARAMS const struct nf_hook_ops *ops, struct sk_buff *skb, \
    const struct net_device *in, const struct net_device *out, int (*okfn) (struct sk_buff *)
#endif
#else
#define KO_NF_HOOK_PARAMS unsigned int hook, struct sk_buff *skb, \
    const struct net_device *in, const struct net_device *out, int (*okfn) (struct sk_buff *)
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
#define KO_NF_PARAM_GET_NET() (state->net)
#define KO_NF_PARAM_GET_HOOKNUM() (state->hook)
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3,0,0)
#define KO_NF_PARAM_GET_NET() NULL
#define KO_NF_PARAM_GET_HOOKNUM() (ops->hooknum)
#else
#define KO_NF_PARAM_GET_NET() NULL
#define KO_NF_PARAM_GET_HOOKNUM() (hook)
#endif


#if LINUX_VERSION_CODE < KERNEL_VERSION(4,4,0)
#define KO_NF_DEF_OPS(_func, _pf, _hooknum, _priority)   \
    {.hook=_func, .owner=THIS_MODULE, .pf=_pf, .hooknum=_hooknum, .priority=(_priority)}

#define KO_NF_FILL_OPS(_ops, _func, _pf, _hooknum, _priority) do { \
    (_ops)->hook = _func; \
    (_ops)->owner=THIS_MODULE; \
    (_ops)->pf = _pf; \
    (_ops)->hooknum = _hooknum; \
    (_ops)->priority = _priority; \
} while(0)

#else
#define KO_NF_DEF_OPS(_func, _pf, _hooknum, _priority)  \
    {.hook=_func, .pf=_pf, .hooknum=_hooknum, .priority=(_priority)}

#define KO_NF_FILL_OPS(_ops, _func, _pf, _hooknum, _priority) do { \
    (_ops)->hook = _func; \
    (_ops)->pf = _pf; \
    (_ops)->hooknum = _hooknum; \
    (_ops)->priority = _priority; \
} while(0)

#endif




#define KO_NF_REG_HOOKS(_opts) nf_register_net_hooks(&init_net, (_opts), ARRAY_SIZE(_opts))
#define KO_NF_UNREG_HOOKS(_opts) nf_unregister_net_hooks(&init_net, (_opts), ARRAY_SIZE(_opts))

#ifdef __cplusplus
}
#endif
#endif 
