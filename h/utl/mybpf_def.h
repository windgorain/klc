/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_DEF_H_
#define _MYBPF_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MYBPF_NAME_SIZE 32
#define MYBPF_FILENAME_SIZE 36  

typedef struct {
    U64 p[5];    
    U64 bpf_ret; 
}MYBPF_PARAM_S;

#ifdef __cplusplus
}
#endif
#endif 
