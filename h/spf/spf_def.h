/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#ifndef _SPF_DEF_H
#define _SPF_DEF_H

#include "utl/mybpf_def.h"
#include "klc/klc_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SPF_HELPER_DO 17

#define KLC_LICENSE_NUM       0x93968798
#define KLC_LICENSE_OUT_NUM   0x98879693

#define KLC_MAPFUNC_INSN_SIZE 4096


enum {
    KLCHELP_CLASS_BASE =  0,
    KLCHELP_CLASS_RUN,
    KLCHELP_CLASS_SKB,

    KLCHELP_CLASS_SYS = 100,
    KLCHELP_CLASS_STR,
    KLCHELP_CLASS_MEM,
};

enum {
    KLCHELP_DO_NOTHING  = KLCHELP_CLASS_BASE << 16,
    KLCHELP_SELF,
    KLCHELP_LOAD,
    KLCHELP_KPRINT_STRING,
    KLCHELP_SNPRINTF,

    
    KLCHELP_BPF_RUN = KLCHELP_CLASS_RUN << 16,
    KLCHELP_ID_LOAD_RUN,
    KLCHELP_NAME_LOAD_RUN,
    KLCHELP_RUN_MAP_PROG,

    KLCHELP_SKB_GET_STRUCT_INFO = KLCHELP_CLASS_SKB << 16, 
    KLCHELP_SKB_GET_INFO,
    KLCHELP_SKB_CREATE,
    KLCHELP_SKB_CONTINUE,
    KLCHELP_SKB_PUT,
    KLCHELP_SKB_RESERVE,
    KLCHELP_SKB_RESET_NETWORK_HEADER,
    KLCHELP_SKB_RESET_TRANSPORT_HEADER,
    KLCHELP_SKB_SET_TRANSPORT_HEADER,
    KLCHELP_SKB_FREE,

    KLCHELP_GET_CGROUP_CLASS_ID = KLCHELP_CLASS_SYS << 16,
    KLCHELP_GET_NAME_FUNC,
    KLCHELP_GET_MAP_BY_PATH,
    KLCHELP_EVENT_PUBLISH,

    KLCHELP_STRLEN = KLCHELP_CLASS_STR << 16,
    KLCHELP_STRNLEN,
    KLCHELP_STRCHR,
    KLCHELP_STRNCHR,
    KLCHELP_STRRCHR,
    KLCHELP_STRSTR,
    KLCHELP_STRNSTR,

    KLCHELP_MEMCPY = KLCHELP_CLASS_MEM << 16,
    KLCHELP_MEMCMP,
    KLCHELP_MEMCHR,
    KLCHELP_MEMMOVE,
    KLCHELP_MEMSET,
    KLCHELP_MEMINVERT,
};

typedef struct {
    char name[MYBPF_NAME_SIZE];
    char filename[MYBPF_FILENAME_SIZE];
}SPF_INSTANCE_S;

typedef struct {
    char name[MYBPF_NAME_SIZE];
    char func_name[MYBPF_NAME_SIZE];
    void *entry;
}SPF_NAMEFUNC_S;

typedef struct {
    U32 id;
    char func_name[MYBPF_NAME_SIZE];
    void *entry;
}SPF_IDFUNC_S;

typedef struct {
    U32 event;
    char func_name[MYBPF_NAME_SIZE];
    void *entry;
}SPF_EVOB_S;

#ifdef __cplusplus
}
#endif
#endif 
