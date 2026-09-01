/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_LOADER_DEF_H
#define _MYBPF_LOADER_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

#define MYBPF_LOADER_FLAG_AUTO_ATTACH 0x1  
#define MYBPF_LOADER_FLAG_REPLACE     0x2  
#define MYBPF_LOADER_FLAG_KEEP_MAP    0x4  
#define MYBPF_LOADER_FLAG_JIT         0x8  
#define MYBPF_LOADER_FLAG_SKIP_JIT_HELPER_CHECK   0x10  

#define MYBPF_LOADER_MAX_MAPS 32
#define MYBPF_LOADER_MAX_PROGS 32

#ifdef __cplusplus
}
#endif
#endif 
