/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _KLCTOOL_DEF_H
#define _KLCTOOL_DEF_H

#include "utl/types.h"
#include "klc/klc_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define KLC_NL_TYPE_MAX 256
#define KLC_NL_TYPE(cmd) (((unsigned int)(cmd) >> 16) & 0xff)
#define KLC_NL_CMD(cmd) ((unsigned int)(cmd) & 0xffff)
#define KLC_NL_TYPE_CMD(type, cmd) (((unsigned int)(type) << 16) | (cmd))

enum {
    KLC_NL_TYPE_CONFIG = 0,
    KLC_NL_TYPE_KV,
    KLC_NL_TYPE_HELPER,
    KLC_NL_TYPE_LOADER,

    KLC_NL_TYPE_SYS,
    KLC_NL_TYPE_NAME_MAP,
};

enum {
    KLC_NL_KO_VER = 0,
    KLC_NL_KO_DECUSE,
    KLC_NL_KO_INCUSE,
    KLC_NL_BASE_DECUSE,
    KLC_NL_BASE_INCUSE,
};

enum {
    KLC_NL_KV_SET = 0,
};

enum {
    KLC_NL_HELPER_SET = 0,
};

enum {
    KLC_NL_LOAD_BARE = 0,
    KLC_NL_LOAD_SPF,
    KLC_NL_UNLOAD_INSTANCE,
    KLC_NL_SPF_RUN_CMD,
    KLC_NL_LOADER_IOCTL,
    KLC_NL_MODULE_IOCTL,
};

enum {
    KLC_NL_MAP_ADD = 0,
};

enum {
    KLC_NL_SYS_GET_OS_HELPER = 0,
    KLC_NL_SYS_DO,
};

enum {
    KLC_NL_MODULE_LOAD_BASE = 0,

    KLC_NL_MODULE_ADD_MOD,
    KLC_NL_MODULE_LOAD_PROG,
    KLC_NL_MODULE_SET_MOD_FUNC, 
    KLC_NL_MODULE_LOADING_EVENT,
    KLC_NL_MODULE_DEL_LOADING,  
    KLC_NL_MODULE_ADD_FINISH,   

    KLC_NL_MODULE_DEL, 

    KLC_INNER_CMD_FINI1,
    KLC_INNER_CMD_FINI2,
};

#ifdef __cplusplus
}
#endif
#endif
