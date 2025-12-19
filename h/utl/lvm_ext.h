/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _LVM_EXT_H_
#define _LVM_EXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LVM_LOCAL_VAR_SIZE 256

typedef struct {
    U8 *data;
    U8 local_var[LVM_LOCAL_VAR_SIZE];
}LVM_S;

static inline void * LVM_MallocExt(LVM_S *pstLvm, U32 size, void *(*mem_alloc_func)(U32))
{
    if (size > LVM_LOCAL_VAR_SIZE) {
        pstLvm->data = mem_alloc_func(size);
    } else {
        pstLvm->data = pstLvm->local_var;
    }
    return pstLvm->data;
}

static inline void LVM_FreeExt(LVM_S *pstLvm, void (*mem_free_func)(void*))
{
    if ((pstLvm->data) && (pstLvm->data != pstLvm->local_var)) {
        mem_free_func(pstLvm->data);
        pstLvm->data = NULL;
    }
}

#ifdef __cplusplus
}
#endif
#endif 
