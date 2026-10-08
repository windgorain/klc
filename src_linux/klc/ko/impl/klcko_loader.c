/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "klcko_impl.h"
#include "ko/ko_nl.h"
#include "ko/ko_utl.h"
#include "klc/klc_nl_def.h"
#include "utl/mybpf_def.h"
#include "utl/mybpf_bare_def.h"
#include "utl/mybpf_spf_def.h"
#include "utl/mybpf_hookpoint_def.h"
#include "utl/arch_utl.h"
#include "klcko_bpf_helper.h"
#include "klcko_bare.h"
#include "klcko_loader.h"

static MYBPF_BARE_S g_klcko_bare;
static MYBPF_SPF_S *g_mybpf_spf_ctrl = NULL;

static int _klcko_loader_load_bare(KLC_LOAD_BARE_NL_S *d)
{
    int ret;
    MYBPF_PARAM_S p = {0};

    ret = MYBPF_LoadBare(d->data, d->len, NULL, &g_klcko_bare);
    if (ret < 0) {
        return ret;
    }

    p.p[0] = (long)d->data;
    MYBPF_RunBareMain(&g_klcko_bare, &p);

    g_mybpf_spf_ctrl = (void*)(long)p.bpf_ret;
    if (! g_mybpf_spf_ctrl) {
        KO_Print("Can't get spf ctrl \n");
        return KO_ERR_FAIL;
    }

    return 0;
}

static int _klcko_loader_load_spf(KLC_LOAD_SPF_NL_S *d)
{
    MYBPF_LOADER_PARAM_S p = {0};
    FILE_MEM_S m;

    if (! g_mybpf_spf_ctrl) {
        KO_Print("Klc not ready\n");
        return KO_ERR_NOT_READY;
    }

    m.data = d->data;
    m.len = d->data_len;

    p.m = &m;
    p.instance = d->instance;
    p.filename = d->filename;
    p.flag = d->flag;
    p.maps_fds = d->maps_fd;
    p.map_count = d->map_count;

    if (g_mybpf_spf_ctrl->load_instance(&p) < 0) {
        KO_Print("Load spf failed \n");
        return KO_ERR_FAIL;
    }

    return 0;
}

static int _klcko_loader_unload_instance(KLC_LOAD_SPF_NL_S *d)
{
    if (! g_mybpf_spf_ctrl) {
        KO_Print("Klc not ready\n");
        return KO_ERR_NOT_READY;
    }
    
    if (d->instance[0] == '\0') {
        g_mybpf_spf_ctrl->unload_all_instance();
        return 0;
    }

    return g_mybpf_spf_ctrl->unload_instance(d->instance);
}

static void _klcko_loader_unload_all_instance(void)
{
    if (! g_mybpf_spf_ctrl) {
        return;
    }

    g_mybpf_spf_ctrl->unload_all_instance();
}

static int _klcko_spf_do_cmd(int argc, U64 *argv, OUT void *reply, int reply_size)
{
    MYBPF_PROG_RUN_S r = {0};
    MYBPF_PARAM_S p = {0};

    if (! g_mybpf_spf_ctrl) {
        KO_Print("Klc not ready\n");
        return KO_ERR_NOT_READY;
    }

    p.p[0] = argc;
    p.p[1] = (long)argv;
    p.p[2] = (long)reply;
    p.p[3] = reply_size;

    r.type = MYBPF_HP_TCMD;
    r.param = &p;

    return g_mybpf_spf_ctrl->run_hookpoint(&r);
}

static int _klcko_loader_run_cmd(KLC_SPF_CMD_NL_S *d, OUT char *reply, int reply_size, OUT int *reply_len)
{
    U64 argv64[KLC_SPF_CMD_ARGC_MAX + 1];
    int i, ret;

    if (d->argc > KLC_SPF_CMD_ARGC_MAX) {
        KO_Print("argc count exceed \n");
        return KO_ERR_BAD_PARAM;
    }

    if (d->argc < 1) {
        KO_Print("No instance name \n");
        return KO_ERR_BAD_PARAM;
    }

    for (i=0; i<d->argc; i++) {
        argv64[i] = (LONG)d->argv[i];
    }

    argv64[d->argc] = 0;

    if (reply_size > 0) {
        reply[0] = '\0';
    }

    ret = _klcko_spf_do_cmd(d->argc, argv64, reply, reply_size);

    if (reply) {
        *reply_len = strlen(reply);
    }

    return ret;
}

static int _klcko_loader_ioctl(KLC_LOADER_IOCTL_S *d, OUT void *reply, int reply_size, OUT int *reply_len)
{
    MYBPF_IOCTL_S io = {0};
    int len;

    if (! g_mybpf_spf_ctrl) {
        KO_Print("Klc not ready\n");
        return KO_ERR_NOT_READY;
    }

    io.in_data = d->in_data;
    io.out_data = reply;
    io.in_data_len = d->in_data_len;
    io.out_data_size = reply_size;

    len = g_mybpf_spf_ctrl->loader_ioctl(d->cmd, &io);
    if (len > 0) {
        *reply_len = len;
    }

    return len;
}

static int _klcko_module_ioctl(KLC_MODULE_IOCTL_S *d, OUT void *reply, int reply_size, OUT int *reply_len)
{
    MYBPF_IOCTL_S io = {0};
    MYBPF_PROG_IOCTL_S io_d = {0};
    int len;

    if (! g_mybpf_spf_ctrl) {
        KO_Print("Klc not ready\n");
        return KO_ERR_NOT_READY;
    }

    io.in_data = d->in_data;
    io.out_data = reply;
    io.in_data_len = d->in_data_len;
    io.out_data_size = reply_size;

    io_d.instance = d->instance;
    io_d.cmd = d->cmd;
    io_d.ioctl = &io;

    len = g_mybpf_spf_ctrl->module_ioctl(&io_d);
    if (len > 0) {
        *reply_len = len;
    }

    return len;
}

static int _klcko_load_nl_do(int cmd, void *data, int data_len, OUT void *reply, int reply_size, OUT int *reply_len)
{
    if (cmd == KLC_NL_LOAD_BARE) {
        return _klcko_loader_load_bare(data);
    } else if (cmd == KLC_NL_LOAD_SPF) {
        return _klcko_loader_load_spf(data);
    } else if (cmd == KLC_NL_UNLOAD_INSTANCE) {
        return _klcko_loader_unload_instance(data);
    } else if (cmd == KLC_NL_SPF_RUN_CMD) {
        return _klcko_loader_run_cmd(data, reply, reply_size, reply_len);
    } else if (cmd == KLC_NL_LOADER_IOCTL) {
        return _klcko_loader_ioctl(data, reply, reply_size, reply_len);
    } else if (cmd == KLC_NL_MODULE_IOCTL) {
        return _klcko_module_ioctl(data, reply, reply_size, reply_len);
    }

    return KO_ERR_BAD_PARAM;
}

int KlcKoLoad_Init(void)
{
    return KlcKoNl_Reg(KLC_NL_TYPE_LOADER, _klcko_load_nl_do);
}

void KlcKoLoad_Fini(void)
{
    KlcKoNl_Reg(KLC_NL_TYPE_LOADER, NULL);
    _klcko_loader_unload_all_instance();
    g_mybpf_spf_ctrl = NULL;
}

