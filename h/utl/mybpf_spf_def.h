/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_SPF_DEF_H_
#define _MYBPF_SPF_DEF_H_

#include "utl/mybpf_ioctl_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *instance __attribute__((aligned(8))); 
    char *filename __attribute__((aligned(8))); 
    FILE_MEM_S *m __attribute__((aligned(8)));
    const void **tmp_helpers __attribute__((aligned(8)));
    int *maps_fds __attribute__((aligned(8)));
    int map_count __attribute__((aligned(8)));  
    U32 flag;
}MYBPF_LOADER_PARAM_S;


typedef struct {
    char *instance __attribute__((aligned(8)));
    char *sec __attribute__((aligned(8)));
    void *prog __attribute__((aligned(8)));
}MYBPF_PROG_ITOR_S;

typedef struct {
    union {
        void *prog;         
        char *name;         
        int type;           
        U32 id;             
        U32 event;          
    }__attribute__((aligned(8)));
    void *prog_end __attribute__((aligned(8))); 
    MYBPF_PARAM_S *param __attribute__((aligned(8)));
}MYBPF_PROG_RUN_S;

typedef struct {
    char *instance __attribute__((aligned(8)));
    int cmd __attribute__((aligned(8)));
    MYBPF_IOCTL_S *ioctl __attribute__((aligned(8)));
}MYBPF_PROG_IOCTL_S;

typedef struct {
    int (*finit)(void) __attribute__((aligned(8)));
    int (*config_by_file)(char *config_file) __attribute__((aligned(8)));
    int (*load_instance)(MYBPF_LOADER_PARAM_S *p) __attribute__((aligned(8)));
    int (*unload_instance)(char *instance_name) __attribute__((aligned(8)));
    void (*unload_all_instance)(void) __attribute__((aligned(8)));
    void * (*get_next_prog)(MYBPF_PROG_ITOR_S *itor) __attribute__((aligned(8)));
    int (*run_prog)(MYBPF_PROG_RUN_S *r) __attribute__((aligned(8)));
    int (*run_hookpoint)(MYBPF_PROG_RUN_S *r) __attribute__((aligned(8)));
    int (*lock_run_hookpoint)(MYBPF_PROG_RUN_S *r) __attribute__((aligned(8)));
    int (*run_bytecode)(MYBPF_PROG_RUN_S *r) __attribute__((aligned(8)));
    int (*run_idfunc)(MYBPF_PROG_RUN_S *r) __attribute__((aligned(8)));
    int (*run_namefunc)(MYBPF_PROG_RUN_S *r) __attribute__((aligned(8)));
    void * (*get_namefunc)(char *name) __attribute__((aligned(8)));
    int (*evob_notify)(MYBPF_PROG_RUN_S *r) __attribute__((aligned(8)));
    void (*set_agent)(U64 id, void *func) __attribute__((aligned(8)));
    int (*loader_ioctl)(U64 cmd, MYBPF_IOCTL_S *d) __attribute__((aligned(8)));
    int (*module_ioctl)(MYBPF_PROG_IOCTL_S *d) __attribute__((aligned(8)));
}MYBPF_SPF_S;

#ifdef __cplusplus
}
#endif
#endif 
