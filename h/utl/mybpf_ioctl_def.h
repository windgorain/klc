/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_IOCTL_DEF_H
#define _MYBPF_IOCTL_DEF_H

#include "utl/mybpf_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    IN void *in_data;   
    OUT void *out_data; 
    IN U32 in_data_len; 
    IN U32 out_data_size; 
}MYBPF_IOCTL_S;

enum {
    SPF_IOCTL_GET_INSTANCE = 0,     
    SPF_IOCTL_INSTANCE_EXIST,       
    SPF_IOCTL_GET_NAMEFUNC,         
    SPF_IOCTL_GET_IDFUNC,           
    SPF_IOCTL_GET_EVOB,             
};

#ifdef __cplusplus
}
#endif
#endif 
