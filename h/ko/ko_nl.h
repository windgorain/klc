/*================================================================
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2017-6-2
* Description: 
* History:     
================================================================*/
#ifndef _KO_NL_H
#define _KO_NL_H

#include "ko/utl/knl_def.h"
#include "ko/ko_errcode.h"

#ifdef __cplusplus
extern "C"
{
#endif

static inline int KO_NL_Reg(void *f, void *o, int count)
{
    int ret;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	ret = genl_register_family_with_ops(f, o, count);  
#else
	ret = genl_register_family(f);
#endif

    return ret;
}

static inline void KO_NL_Unreg(void *f, void *o)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
    genl_unregister_ops(f, o);
#endif
    genl_unregister_family(f); 
}

static inline int KO_NL_GetOne(void *node, PF_KUTL_NL_FILL nl_fill_fn, void *reply, void *ud)
{
    int ret;
    int user_size;
    KNL_GET_S __user *out = reply;

    if (! out) {
        return KO_ERR_BAD_PARAM;
    }

    if (! node) {
        if (put_user(0, (int __user *)&out->count)) {
            return -EFAULT;
        }
        return KO_ERR_NO_SUCH;
    }

    ret = get_user(user_size, (int __user *)&out->ele_size);
    if (ret != 0) {
        return -EFAULT;
    }

    if (nl_fill_fn(node, out + 1, user_size, ud) < 0) {
        return -EFAULT;
    }

    if (put_user(1, (int __user *)&out->count)) {
        return -EFAULT;
    }

    return 0;
}

static inline int KO_NL_Dump(void *reply, void *data, int data_size)
{
    KNL_DUMP_S __user *out = reply;
    int dump_size;
    int write_size;

    if (! out) {
        return KO_ERR_BAD_PARAM;
    }

    if (get_user(dump_size, (int __user *)&out->dump_size)) {
        return -EFAULT;
    }

    if (dump_size <= 0) {
        return KO_ERR_BAD_PARAM;
    }

    if (! data) {
        if (put_user(0, (int __user *)&out->write_size)) {
            return -EFAULT;
        }
        return 0;
    }

    write_size = MIN(dump_size, data_size);

    if (copy_to_user(out->data, data, write_size)) {
        return -EFAULT;
    }

    if (put_user(write_size, (int __user *)&out->write_size)) {
        return -EFAULT;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
#endif 
