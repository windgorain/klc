/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_AOT_DEF_H
#define _MYBPF_AOT_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct {
    const void **base_helpers __attribute__((aligned(8)));
    const void **sys_helpers __attribute__((aligned(8)));
    const void **user_helpers __attribute__((aligned(8)));
    const void **tmp_helpers __attribute__((aligned(8)));
    void *maps __attribute__((aligned(8))); 
    void **global_map_data __attribute__((aligned(8)));
    void *loader_node __attribute__((aligned(8)));
}MYBPF_AOT_PROG_CTX_S;

#ifdef __cplusplus
}
#endif
#endif 
