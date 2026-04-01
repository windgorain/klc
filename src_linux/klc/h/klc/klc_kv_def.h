/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _KLC_KV_DEF_H
#define _KLC_KV_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

enum {
    KLC_KV_SET_MEM_X = 0,
    KLC_KV_SET_MEM_NX,
    KLC_KV_SET_MEM_RO,
    KLC_KV_SET_MEM_RW,
    KLC_KV_MODULE_ALLOC,
    KLC_KV_MODULE_FREE,
    KLC_KV_BPF_MAP_LOOKUP,
    KLC_KV_BPF_MAP_UPDATE,
    KLC_KV_BPF_MAP_DELETE,
    KLC_KV_FIND_VM_AREA,
    KLC_KV_JIT_ALLOC,
    KLC_KV_JIT_FREE,

    KLC_KV_MAX = 1024
};

#ifdef __cplusplus
}
#endif
#endif 
