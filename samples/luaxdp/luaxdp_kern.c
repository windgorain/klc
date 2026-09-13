/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang
* Description: 
* History:     
******************************************************************************/
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include "utl/types.h"
#include "utl/int_types.h"
#include "klc/klc_def.h"
#include "spf/klc/klc_help.h"
#include "klc/klc_utl.h"
#include "spf/spf_def.h"
#include "luaxdp_def.h"

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, int);
    __type(value, LUAXDP_CODE_S);
    __uint(max_entries, 1);
    __uint(pinning, 1);
} g_luaxdp_code_cfg SEC(".maps");

static inline void * _luaxdp_load(void)
{
    int id = 0;
    LUAXDP_CODE_S *code;

    code = bpf_map_lookup_elem(&g_luaxdp_code_cfg, &id);
    if ((! code) || (! code->valid)) {
        return NULL;
    }

    void *L = (void*)KLCHLP_NameRun("luaL_newstate", 0);
    if (! L) {
        return NULL;
    }

    KLCHLP_NameRun("luaL_openlibs", 0, L);

    if (KLCHLP_NameRun("luaL_dostring", -1, L, code->lua_code) != 0) {  
        char str[] = "";
        const char *message = (void*)KLCHLP_NameRun("lua_tostring", (long)str, L, -1);  
        BPF_Print("Load lua script error: %s\n", message);
        KLCHLP_NameRun("lua_pop", 0, L, 1);  
        KLCHLP_NameRun("lua_close", 0, L);
        return NULL;
    }

    return L;
}

static inline void _luaxdp_print_host(void *L)
{
    int is_string = KLCHLP_NameRun("lua_isstring", 0, L, -1);

    if (is_string) {
        char hoststr[64];
        long hlen = 0;
        const char *p = (void*)(long)KLCHLP_NameRun("lua_tolstring", 0, L, -1, &hlen);
        int len = MIN(hlen, sizeof(hoststr) - 1);
        
        if (p) {
            KLCHLP_Memcpy(hoststr, p, len);
            hoststr[len] = '\0';
            BPF_Print("Host: %s", hoststr);
        }
    }

    
    KLCHLP_NameRun("lua_pop", 0, L, 1);
}

static inline void _luaxdp_do(void *ctx, void *data, void *data_end)
{
    void *L = _luaxdp_load();
    if (! L) {
        return;
    }

    char func[] = "lua_input";

    KLCHLP_NameRun("lua_getglobal", 0, L, func);
    KLCHLP_NameRun("lua_pushlstring", 0, L, data, data_end - data);
    KLCHLP_NameRun("lua_pushinteger", 0, L, data_end - data);
    KLCHLP_NameRun("lua_pcall", -1, L, 2, 1, 0);
    _luaxdp_print_host(L);
    KLCHLP_NameRun("lua_close", 0, L);
}

SEC("xdp")
int test_xdp_input(struct xdp_md *ctx)
{
    KLCHLP_License();

    void *data = (void*)(long)ctx->data;
    void *data_end = (void*)(long)ctx->data_end;

    _luaxdp_do(ctx, data, data_end);

    return XDP_PASS;
}

char _license[] SEC("license") = "Dual BSD/GPL";

