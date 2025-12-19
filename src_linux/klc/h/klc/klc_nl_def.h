/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _KLC_NL_DEF_H
#define _KLC_NL_DEF_H
#include "utl/mybpf_def.h"
#include "utl/mybpf_loader_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define KLC_LOADER_MAX_MAPS 32

typedef struct {
    char map_name[KLC_NAME_SIZE];
	unsigned int type;
	unsigned int size_key;
	unsigned int size_value;
	unsigned int max_elem;
	unsigned int flags;
}KLC_MAP_DEF_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    char version[KLC_NAME_SIZE];
    char compile_time[KLC_NAME_SIZE];
}KLC_ADD_MOD_NL_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    int map_count;
    KLC_MAP_DEF_S maps_def[KLC_LOADER_MAX_MAPS];
}KLC_LOAD_MAPS_NL_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    int map_id;
    unsigned int flag;
    int key_size;
    int value_size;
    char key_value[0]; 
}KLC_UPDATE_MAP_ELE_NL_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    int map_id;
    int key_size;
    int value_size;
    void *key_ptr;
    char key[0]; 
}KLC_LOOKUP_MAP_ELE_NL_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    int map_id;
    int key_size;
    void *key_ptr;
    char key[0]; 
}KLC_MAP_ELE_KEY_NL_S;

#define KLC_LOAD_BASE_FIN_NUM 2

typedef struct {
    void *init_it[2];
    void *fin_it[KLC_LOAD_BASE_FIN_NUM];
    int data_len;
    char data[0];
}KLC_LOAD_BASE_NL_S;

typedef struct {
    U32 id;
    U64 value;
}KLC_KV_SET_NL_S;

typedef struct {
    U64 len;
    char data[0];
}KLC_LOAD_BARE_NL_S;

typedef struct {
    char instance[MYBPF_NAME_SIZE];
    char filename[MYBPF_NAME_SIZE];
    U32 flag;
    U32 map_count;
    int maps_fd[MYBPF_LOADER_MAX_MAPS];
    U64 data_len;
    char data[0];
}KLC_LOAD_SPF_NL_S;

#define KLC_SPF_CMD_ARGC_MAX 16
typedef struct {
    U32 argc;
    char argv[KLC_SPF_CMD_ARGC_MAX][128];
}KLC_SPF_CMD_NL_S;

typedef struct {
    int cmd;
    U32 in_data_len;
    U8 in_data[0];
}KLC_LOADER_IOCTL_S;

typedef struct {
    char instance[KLC_NAME_SIZE];
    int cmd;
    U32 in_data_len;
    U8 in_data[0];
}KLC_MODULE_IOCTL_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    int insn_len; 
    char insn[0];
}KLC_LOAD_PROG_NL_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
}KLC_LOAD_FINISH_NL_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
}KLC_NL_MOD_REQ_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    char version[KLC_NAME_SIZE];
    char compile_time[KLC_NAME_SIZE];
    void *mod;
    void *insts;
    int maps_count;
    int global_data_count;
    void *maps[KLC_LOADER_MAX_MAPS];
    void *global_data[KLC_LOADER_MAX_MAPS];
}KLC_NL_MOD_REPLY_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    int maps_count;
    KLC_MAP_DEF_S maps_def[KLC_LOADER_MAX_MAPS];
}KLC_NL_MOD_MAPS_REPLY_S;

#if 1
enum {
    KLC_MOD_FUNC_TYPE_CMDRUN = 0,
    KLC_MOD_FUNC_TYPE_MOD_EVENT,
};

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    U32 prog_offset;
    U8 type;
}KLC_MOD_FUNC_NL_S;
#endif

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    int event;
}KLC_MOD_LOADING_EVENT_NL_S;

#define KLC_MOD_CMD_RUN_ARGC_MAX 32
typedef struct {
    char mod_name[KLC_NAME_SIZE];
    U8 argc;
    char argv[KLC_MOD_CMD_RUN_ARGC_MAX][64];
}KLC_MOD_CMD_RUN_NL_S;

typedef struct {
    char name[KLC_NAME_SIZE];
    unsigned int id;
}KLC_FUNC_GET_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    char map_name[KLC_NAME_SIZE];
    int fd;
}KLC_NAMEMAP_ADD_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    char map_name[KLC_NAME_SIZE];
}KLC_NAMEMAP_GET_S;

typedef struct {
    unsigned int start_id;
    unsigned int end_id;
}KLC_OSHELPER_GET_S;

typedef struct {
    U32 id;
    U32 is_off; 
    U64 p[4];
    U64 ret;
    char data[1024];
}KLC_SYS_DO_S;

typedef struct {
    char mod_name[KLC_NAME_SIZE];
    char map_name[KLC_NAME_SIZE];
    int map_id;
    char key[0];
}KLC_KMAP_LOOKUP_ELE_NL_S;

#ifdef __cplusplus
}
#endif
#endif 
