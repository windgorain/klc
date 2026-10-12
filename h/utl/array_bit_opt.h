/******************************************************************************
* Copyright (C), Xingang.Li
* Version: 1.0  Date: 2007-8-18
* Description: 
******************************************************************************/
#ifndef _ARRAY_BIT_OPT_H_
#define _ARRAY_BIT_OPT_H_

#ifdef __cplusplus
extern "C" {
#endif

static inline void ArrayBit_Set(U32 *data, U32 index)
{
    data[index>>5] |= ((U32)1 << (index & 31));
}

static inline void ArrayBit_Clr(U32 *data, U32 index)
{
    data[index>>5] &= ~((U32)1 << (index & 31));
}

static inline int ArrayBit_Test(U32 *data, U32 index)
{
    U32 ret = data[index>>5] & ((U32)1 << (index & 31));
    return !!ret;
}

#ifdef __cplusplus
}
#endif
#endif 
