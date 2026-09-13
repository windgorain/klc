/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _SPF_MODULE_H_
#define _SPF_MODULE_H_

#include "utl/mybpf_ioctl_def.h"

#ifdef __cplusplus
extern "C" {
#endif

void * _memcpy(void *dest, const void *src, size_t count);



#define SPF_MODINFO_DEF(_ver) \
    static SPF_MOD_INFO_S _g_mod_info = {.ver = (_ver), \
        .compile_time = __DATE__ " "  __TIME__, \
    }

static inline int _spfmod_build_info(OUT MYBPF_IOCTL_S *d, void *data, int data_size)
{
    int len = MIN((int)d->out_data_size, data_size);
    _memcpy(d->out_data, data, len);
    return len;
}

#define SPF_MOD_COMMON_CASE \
        case SPF_MOD_IOCTL_GET_MOD_INFO: \
            return _spfmod_build_info(d, &_g_mod_info, sizeof(_g_mod_info));

#define SPF_MOD_COMMON_DEF(_ver) \
    SPF_MODINFO_DEF(_ver); \
    SEC(SPF_SEC_MODULE_IOCTL) \
    int __dft_module_ioctl(int cmd, MYBPF_IOCTL_S *d) { \
    switch (cmd) { \
        SPF_MOD_COMMON_CASE \
    } \
    return 0; \
}

#ifdef __cplusplus
}
#endif
#endif 
