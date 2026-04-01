/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2022-7-16
* Description:
******************************************************************************/
#include "klcko_impl.h"
#include "ko/ko_nl.h"
#include "ko/ko_utl.h"
#include "klc/klc_nl_def.h"
#include "klc/klc_kv_def.h"
#include "utl/arch_utl.h"
#include "klcko_bpf_helper.h"
#include "klcko_bare.h"
#include "klcko_kv.h"

typedef void (*PF_INIT_IT)(void *p, int num);

static void ** _mybpf_bare_malloc_bss(int bss_size)
{
    void **bss = kmalloc(16 + bss_size, GFP_ATOMIC);
    if (! bss) {
        return NULL;
    }

    memset(bss, 0, 16 + bss_size);

    bss[0] = (char*)bss + 16;

    return bss;
}

static void _mybpf_bare_free_bss(void **bss)
{
    if (bss) {
        kfree(bss);
    }
}

static int _mybpf_bare_check_depends(MYBPF_BARE_HDR_S *hdr, const void **tmp_helpers)
{
    MYBPF_BARE_SUB_HDR_S *shdr = (void*)(hdr + 1);
    int *helpers;
    int depend_count;
    int i;

    depend_count = ntohl(shdr->depends_count);
    if (depend_count == 0) {
        return 0;
    }

    helpers = (void*)(shdr + 1);

    for (i=0; i<depend_count; i++) {
        int helper_id = ntohl(helpers[i]);
        void *fn = ulc_get_helper(helper_id, tmp_helpers);
        if (! fn) {
            KO_Print("Not support helper %d\n", helper_id);
            return KO_ERR_NO_SUCH;
        }
    }

    return 0;
}


static int _mybpf_bare_check(MYBPF_BARE_HDR_S *hdr, int mem_len, const void **tmp_helpers)
{
    int size;

    if (mem_len <= sizeof(*hdr)) {
        
        KO_Print("Bare file too small\n");
        return KO_ERR_FAIL;
    }

    if (hdr->magic != htonl(MYBPF_BARE_MAGIC)) {
        
        KO_Print("Magic not match\n");
        return KO_ERR_FAIL;
    }

    size = ntohl(hdr->total_size);
    if (size > mem_len) {
        KO_Print("Bare file too small\n");
        return KO_ERR_FAIL;
    }

    if ((! hdr->arch) || (hdr->arch != ARCH_LocalArch())) {
        KO_Print("Jit arch not matched\n");
        return KO_ERR_FAIL;
    }

    return _mybpf_bare_check_depends(hdr, tmp_helpers);
}

static void * _mybpf_bare_alloc(int size)
{
    void * (*jit_alloc)(int)= KLCKO_GetKV(KLC_KV_JIT_ALLOC);
    if (! jit_alloc) {
        return NULL;
    }

    return jit_alloc(size);
}

static void _mybpf_bare_free(void *mem)
{
    void (*jit_free)(void*)= KLCKO_GetKV(KLC_KV_JIT_FREE);
    if (! jit_free) {
        return;
    }

    jit_free(mem);
}

static int _mybpf_bare_make_exe(void *fn, int pages)
{
    int (*func1)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_RO); 
    int (*func2)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_X); 
    void * (*func3)(void *) = KLCKO_GetKV(KLC_KV_FIND_VM_AREA);

    if ((! func1) || (! func2) || (! func3)) {
        KO_Print("Can't get func\n");
        return KO_ERR_FAIL;
    }

    struct vm_struct *vm = func3(fn);
    if (vm) {
		vm->flags |= VM_FLUSH_RESET_PERMS;
    }

    int ret = func1(fn, pages);
    if (ret < 0) {
        KO_Print("func1 error:%d\n", ret);
        return ret;
    }

    ret = func2(fn, pages);
    if (ret < 0) {
        KO_Print("func2 error:%d\n", ret);
        return ret;
    }

    return ret;
}

static int _mybpf_bare_load(void *data, int len, const void **tmp_helpers, OUT MYBPF_BARE_S *bare)
{
    void **bss = NULL;
    int bss_size;
    int shdr_len;
    int prog_size;
    int alloc_size;
    int pages;
    void *prog_begin;
    void *fn;
    MYBPF_BARE_HDR_S *hdr = data;
    MYBPF_BARE_SUB_HDR_S *shdr = (void*)((char*)hdr + ntohs(hdr->hdr_size));

    int ret = _mybpf_bare_check(hdr, len, tmp_helpers);
    if (ret < 0) {
        return ret;
    }

    bss_size = ntohl(shdr->bss_size);
    if (bss_size) {
        bss = _mybpf_bare_malloc_bss(bss_size);
        if (! bss) {
            KO_Print("Can't malloc bss memory\n");
            return KO_ERR_NO_MEMORY;
        }
    }

    shdr_len = ntohs(shdr->sub_hdr_size) + (sizeof(int) * ntohl(shdr->depends_count));
    prog_begin = (char*)shdr + shdr_len;
    prog_size = ntohl(shdr->sub_size) - shdr_len;
    alloc_size = round_up(prog_size, PAGE_SIZE);
    pages = alloc_size / PAGE_SIZE;

    
    fn = _mybpf_bare_alloc(alloc_size);
    if (! fn) {
        _mybpf_bare_free_bss(bss);
        KO_Print("Can't malloc prog memory\n");
        return KO_ERR_FAIL;
    }

    memcpy(fn, prog_begin, prog_size);
    if (_mybpf_bare_make_exe(fn, pages) < 0) {
        _mybpf_bare_free_bss(bss);
        _mybpf_bare_free(fn);
        KO_Print("Make exe failed\n");
        return KO_ERR_FAIL;
    }

    bare->prog = fn;
    bare->prog_len = prog_size;
    bare->ctx.base_helpers = ulc_get_base_helpers();
    bare->ctx.sys_helpers = ulc_get_sys_helpers();
    bare->ctx.user_helpers = ulc_get_user_helpers();
    bare->ctx.global_map_data = bss;
    bare->ctx.tmp_helpers = tmp_helpers;

    return 0;
}

static U64 _mybpf_bare_call(MYBPF_BARE_S *bare, void *func, MYBPF_PARAM_S *p)
{
    U64 (*fn)(U64, U64, U64, U64, U64, void*) = func;
    p->bpf_ret = fn(p->p[0], p->p[1], p->p[2], p->p[3], p->p[4], &bare->ctx);
    return p->bpf_ret;
}

int MYBPF_LoadBare(void *data, int len, const void **tmp_helpers, OUT MYBPF_BARE_S *bare)
{
    return _mybpf_bare_load(data, len, tmp_helpers, bare);
}

U64 MYBPF_RunBareMain(MYBPF_BARE_S *bare, MYBPF_PARAM_S *p)
{
    return _mybpf_bare_call(bare, bare->prog, p);
}

void MYBPF_UnloadBare(MYBPF_BARE_S *bare)
{
    if (bare) {
        _mybpf_bare_free_bss(bare->ctx.global_map_data);
        _mybpf_bare_free(bare->prog);
        memset(bare, 0, sizeof(*bare));
    }
}
