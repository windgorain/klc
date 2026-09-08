/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _KLC_HELP_H
#define _KLC_HELP_H

#include "utl/args_def.h"
#include "utl/mybpf_utl.h"
#include "spf/spf_def.h"

static U64 (*_klc_helper)(U64 type, U64 p2, U64 p3, U64 p4, U64 p5) = (void*)SPF_HELPER_DO;

static __always_inline void KLCHLP_DoNothing()
{
    _klc_helper(KLCHELP_DO_NOTHING, 0, 0, 0, 0);
}

static __always_inline void KLCHLP_License()
{
    int license[] = {KLC_LICENSE_NUM};
    _klc_helper(KLCHELP_DO_NOTHING, (long)(void*)license, 0, 0, 0);
}

static __always_inline unsigned int KLCHLP_GetCGroupClassID(void *skb)
{
    U64 ret = _klc_helper(KLCHELP_GET_CGROUP_CLASS_ID, (long)skb, 0, 0, 0);
    if (ret < 0) {
        return 0;
    }
    return ret;
}

static __always_inline U64 _KLCHLP_Self(void *v, int v_size)
{
    return _klc_helper(KLCHELP_SELF, (long)v, v_size, 0, 0);
}

#define KLCHLP_Self(_v) _KLCHLP_Self((_v), sizeof(*(_v)));


#define KLC_GET(P) ({typeof(P) _val=0; KLCHLP_Memcpy(&_val, &(P), sizeof(_val)); KLCHLP_Self(&_val);})

static __always_inline void * KLCHLP_SkbCreate(int data_len)
{
    U64 ret = _klc_helper(KLCHELP_SKB_CREATE, data_len, 0, 0, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)(long)ret;
}

static __always_inline void KLCHLP_SkbFree(void *skb)
{
    _klc_helper(KLCHELP_SKB_FREE, (long)skb, 0, 0, 0);
}


static __always_inline int SKBKLC_GetSkbInfo(void *skb, OUT KLC_SKB_INFO_S *info)
{
    if ((!skb) || (!info)) {
        return -1;
    }

    return _klc_helper(KLCHELP_SKB_GET_INFO, (long)skb, (long)info, 0, 0);
}


static __always_inline void * KLCHLP_GetSkbData(void *skb)
{
    KLC_SKB_INFO_S info = {0};

    if (SKBKLC_GetSkbInfo(skb, &info)) {
        return NULL;
    }

    return (void*) KLCHLP_Self(&info.data);
}

static __always_inline BOOL_T KLCHLP_SkbContinue(void *skb, int len)
{
    if (! skb) {
        return 0;
    }

    U64 ret = _klc_helper(KLCHELP_SKB_CONTINUE, (long)skb, (long)len, 0, 0);
    if (ret == KLC_RET_ERR) {
        return 0;
    }

    return ret;
}

static __always_inline U64 KLCHLP_SkbPut(void* skb, int len) 
{
    if(!skb) {
        return 0;
    }

    U64 ret = _klc_helper(KLCHELP_SKB_PUT, (long)skb, (long)len, 0, 0);
    if(ret == KLC_RET_ERR) {
	return 0;
    }
    return ret;
}

static __always_inline U64 KLCHLP_SkbReserve(void* skb, int len) 
{
    U64 ret = _klc_helper(KLCHELP_SKB_RESERVE, (long)skb, (long)len, 0, 0);
    if(ret == KLC_RET_ERR) {
	return 0;
    }
    return ret;
}

static __always_inline U64 KLCHLP_SkbResetNetworkHeader(void* skb) 
{
    U64 ret = _klc_helper(KLCHELP_SKB_RESET_NETWORK_HEADER, (long)skb, 0, 0, 0);
    if(ret == KLC_RET_ERR) {
	return 0;
    }
    return ret;
}

static __always_inline U64 KLCHLP_Skb_ResetTransport_Header(void* skb)
{
    U64 ret = _klc_helper(KLCHELP_SKB_RESET_TRANSPORT_HEADER, (long)skb, 0, 0, 0);
    if(ret == KLC_RET_ERR) {
        return 0;
    }
    return ret;
}
static __always_inline U64 KLCHLP_Skb_Set_Transport_Header(void* skb, int len)
{
    U64 ret = _klc_helper(KLCHELP_SKB_SET_TRANSPORT_HEADER, (long)skb, (long)len, 0, 0);
    if(ret == KLC_RET_ERR) {
        return 0;
    }
    return ret;
}

static __always_inline int _KLCHLP_EventPublish(unsigned int event, MYBPF_PARAM_S *p)
{
    return _klc_helper(KLCHELP_EVENT_PUBLISH, event, (long)p, 0, 0);
}


#define KLCHLP_EventPub(_err_ret, _event, ...) ({ \
        MYBPF_PARAM_S _p = {0}; _p.bpf_ret = (_err_ret);\
        _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
        _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
        _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
        _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
        _KLCHLP_EventPublish(_event, &_p); \
        KLCHLP_Self(&_p.bpf_ret); \
        })

static __always_inline U64 KLCHLP_Load(void *mem, unsigned int count )
{
    U64 ret;

    if (count > 8) {
        return KLC_RET_ERR;
    }

    ret = _klc_helper(KLCHELP_LOAD, (long)mem, count, 0, 0);

    return ret;
}

static __always_inline int KLCHLP_LoadX4(void *mem, OUT void *buf, int count)
{
    U64 ret;
    volatile unsigned int *b4 = buf;
    volatile unsigned short *b2;
    volatile unsigned char *b1;
    char *m = mem;
    int i;
    int loop_count;
    int have2;
    int have1;

    loop_count = count / 4;
    have2 = (count % 4) / 2;
    have1 = count % 2;

    
#pragma unroll
    for (i=0; i<loop_count; i++) {
        ret = KLCHLP_Load(m, 4);
        if (KLC_RET_ERR == ret) {
            return -1;
        }
        *b4 = ret;
        m += 4;
        b4 ++;
    }

    
    b2 = (void*)b4;
    if (have2) {
        ret = KLCHLP_Load(m, 2);
        if (ret == KLC_RET_ERR) {
            return -1;
        }
        *b2 = ret;
        m += 2;
        b2 ++;
    }

    
    b1 = (void*)b2;
    if (have1) {
        ret = KLCHLP_Load(m, 1);
        if (ret == KLC_RET_ERR) {
            return -1;
        }
        *b1 = ret;
    }

    return 0;
}


static __always_inline int KLCHLP_LoadX8(void *mem, OUT void *buf, int count)
{
    U64 ret;
    volatile U64 *p = buf;
    char *m = mem;
    int i;
    int loop;
    int left;
    int may_err = 0;

    loop = count / 8;
    left = count % 8;

    
#pragma unroll
    for (i=0; i<loop; i++) {
        ret = KLCHLP_Load(m, 8);
        if (ret == KLC_RET_ERR) {
            may_err = 1;
            break;
        }
        *p = ret;
        p ++;
        m += 8;
    }

    if (may_err) {
        return KLCHLP_LoadX4(mem, buf, count);
    }

    return KLCHLP_LoadX4(m, (void*)p, left);
}

static __always_inline int KLCHLP_LoadX(void *mem, OUT void *buf, int count)
{
    return KLCHLP_LoadX8(mem, buf, count);
}

static __always_inline int KLCHLP_Strlen(char *buf)
{
    U64 ret = _klc_helper(KLCHELP_STRLEN, (long)buf, 0, 0, 0);
    if (ret == KLC_RET_ERR) {
        return 0;
    }
    return (long)ret;
}

static __always_inline int KLCHLP_Strnlen(char *buf, int len)
{
    U64 ret = _klc_helper(KLCHELP_STRNLEN, (long)buf, len, 0, 0);
    if (ret == KLC_RET_ERR) {
        return 0;
    }
    return (long)ret;
}

static __always_inline char * KLCHLP_Strchr(char *s, int c)
{
    U64 ret = _klc_helper(KLCHELP_STRCHR, (long)s, (long)c, 0, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

static __always_inline char * KLCHLP_Strnchr(char *s, int count, int c)
{
    U64 ret = _klc_helper(KLCHELP_STRNCHR, (long)s, (long)count, (long)c, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

static __always_inline char * KLCHLP_Strrchr(char *s, int c)
{
    U64 ret = _klc_helper(KLCHELP_STRRCHR, (long)s, (long)c, 0, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

static __always_inline char * KLCHLP_Strstr(char *buf, char *to_find)
{
    U64 ret = _klc_helper(KLCHELP_STRSTR, (long)buf, (long)to_find, 0, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

static __always_inline char * KLCHLP_Strnstr(char *buf, char *to_find, int len)
{
    U64 ret = _klc_helper(KLCHELP_STRNSTR, (long)buf, (long)to_find, (long)len, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

static __always_inline int _klchlp_snprintf(char *buf, int size, char *fmt, IN KLC_PARAM_S *p)
{
    U64 ret = _klc_helper(KLCHELP_SNPRINTF, (long)buf, size, (long)fmt, (long)p);
    if (ret == KLC_RET_ERR) {
        return -1;
    }
    return ret;
}


#define KLCHLP_Snprintf(buf,size,fmt,...) ({ \
    KLC_PARAM_S p={0}; int ret=-1;\
    char _fmt[] = fmt; \
    p.count = BS_ARG_COUNT(__VA_ARGS__); \
    switch (p.count) { \
        case 5: p.param[4]=(unsigned long long)BS_ARG_GET(5, __VA_ARGS__); \
        case 4: p.param[3]=(unsigned long long)BS_ARG_GET(4, __VA_ARGS__); \
        case 3: p.param[2]=(unsigned long long)BS_ARG_GET(3, __VA_ARGS__); \
        case 2: p.param[1]=(unsigned long long)BS_ARG_GET(2, __VA_ARGS__); \
        case 1: p.param[0]=(unsigned long long)BS_ARG_GET(1, __VA_ARGS__); \
        case 0: break; \
    } \
    if (p.count <= 5) { ret = _klchlp_snprintf(buf,size,_fmt,&p);} \
    ret; \
})

static __always_inline void * KLCHLP_Memchr(void *s, int c, int n)
{
    U64 ret = _klc_helper(KLCHELP_MEMCHR, (long)s, (long)c, (long)n, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

static __always_inline int KLCHLP_Memcmp(const void *cs, const void *ct, int count)
{
    U64 ret = _klc_helper(KLCHELP_MEMCMP, (long)cs, (long)ct, (long)count, 0);
    if (ret == KLC_RET_ERR) {
        return -1;
    }
    return ret;
}


static __always_inline int KLCHLP_Memcpy(void *dst, const void *src, int len)
{
    U64 ret = _klc_helper(KLCHELP_MEMCPY, (long)dst, (long)src, len, 0);
    if (ret == KLC_RET_ERR) {
        return -1;
    }
    return 0;
}

static __always_inline void * KLCHLP_Memmove(void *dst, void *src, int count)
{
    U64 ret = _klc_helper(KLCHELP_MEMMOVE, (long)dst, (long)src, count, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

static __always_inline int KLCHLP_Memset(void *dst, int c, int count)
{
    U64 ret = _klc_helper(KLCHELP_MEMSET, (long)dst, c, count, 0);
    if (ret == KLC_RET_ERR) {
        return -1;
    }
    return 0;
}

static __always_inline int KLCHLP_MemInvert(void *in, int len, OUT void *out)
{
    U64 ret = _klc_helper(KLCHELP_MEMINVERT, (long)in, len, (long)out, 0);
    if (ret == KLC_RET_ERR) {
        return -1;
    }
    return 0;
}

static __always_inline void _KLCHLP_KoPrint(char *str, U64 p1, U64 p2, U64 p3)
{
    _klc_helper(KLCHELP_KPRINT_STRING, (long)str, p1, p2, p3);
}

#define KLCHLP_KoPrint(str, ...) do { \
    char info[] = str; \
    _KLCHLP_KoPrint(info, BS_ARG_GET(1, ##__VA_ARGS__), BS_ARG_GET(2, ##__VA_ARGS__), BS_ARG_GET(3, ##__VA_ARGS__)); \
}while(0)

#define KLCHLP_KoPrintLn() do { \
    KLCHLP_KoPrint("%d \n", __LINE__); \
}while(0)

#ifndef BPF_Print
#define BPF_Print(_fmt, ...) ({ \
    char _msg[] = _fmt; \
    bpf_trace_printk(_msg, sizeof(_msg), ##__VA_ARGS__); \
})
#endif


static __always_inline void KLCHLP_PrintL(char *str, int len)
{
    char info[32] = "";

    int copy_len = MIN(sizeof(info)-1, len);
    if (0 != KLCHLP_Memcpy(info, str, copy_len)) {
        return;
    }

    BPF_Print("%s", info);
}

static __always_inline void * _KLCHLP_GetNameFunc(char *name)
{
    U64 ret = _klc_helper(KLCHELP_GET_NAME_FUNC, (long)name, 0, 0, 0);
    if (ret == KLC_RET_ERR) {
        return NULL;
    }
    return (void*)ret;
}

#define KLCHLP_GetLocalNameFunc(name) ({ \
    char _name[KLC_NAME_SIZE] = KLC_MODULE_NAME "/" name; \
    _KLCHLP_GetNameFunc(_name); \
})

static __always_inline void * KLCHLP_GetFuncEntry(void *func)
{
    return func;
}

static __always_inline int _KLCHLP_BpfRun(void *bpf, int bpfsize, MYBPF_PARAM_S *p)
{
    return _klc_helper(KLCHELP_BPF_RUN, (long)bpf, (long)bpf + bpfsize, (long)p, 0);
}

#define KLCHLP_BPF_RUN(_err_ret, _bpf, _bpfsize, ...) ({ \
        U64 _ret = (_err_ret); \
        MYBPF_PARAM_S _p = {0}; \
        _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
        _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
        _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
        _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
        _p.p[4] = (long)BS_ARG_GET(5, ##__VA_ARGS__); \
        if (_KLCHLP_BpfRun(_bpf, _bpfsize, &_p) >= 0) _ret = KLCHLP_Self(&_p.bpf_ret); \
        _ret; })

static __always_inline U64 _KLCHLP_IDRun(U64 id, long err_ret, MYBPF_PARAM_S *p)
{
    long ret = _klc_helper(KLCHELP_ID_LOAD_RUN, id, (long)p, 0, 0);
    if (ret < 0) {
        
        return err_ret;
    }
    
    return KLCHLP_Self(&p->bpf_ret);
}

#define KLCHLP_IDRun(_id, _err_ret, ...) ({ \
    MYBPF_PARAM_S _p; _p.bpf_ret = 0; \
    _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
    _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
    _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
    _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
    _p.p[4] = (long)BS_ARG_GET(5, ##__VA_ARGS__); \
    _KLCHLP_IDRun(_id, _err_ret, &_p); \
})

static __always_inline U64 _KLCHLP_NameRun(char *name, long err_ret, MYBPF_PARAM_S *p)
{
    long ret = _klc_helper(KLCHELP_NAME_LOAD_RUN, (long)name, (long)p, 0, 0);
    if (ret < 0) { 
        
        return err_ret;
    }
    
    return KLCHLP_Self(&p->bpf_ret); 
}

#define KLCHLP_NameRun(name, _err_ret, ...) ({ \
    char _name[KLC_NAME_SIZE] = name; \
    MYBPF_PARAM_S _p; _p.bpf_ret = 0; \
    _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
    _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
    _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
    _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
    _p.p[4] = (long)BS_ARG_GET(5, ##__VA_ARGS__); \
    _KLCHLP_NameRun(_name, _err_ret, &_p); \
})

static __always_inline int KLCHLP_MapRunExt(void *map, void *key, MYBPF_PARAM_S *p)
{
    void *code = bpf_map_lookup_elem(map, key);
    if (! code) {
        return -1;
    }
    return _KLCHLP_BpfRun(code, KLC_MAPFUNC_INSN_SIZE, p);
}

#define KLCHLP_MAP_RUN(_err_ret, _map, _key, ...) ({ \
        U64 _ret = (_err_ret); \
        void *_code = bpf_map_lookup_elem(_map, _key); \
        if (_code) { \
            MYBPF_PARAM_S _p; _p.bpf_ret = 0; \
            _p.p[0] = (long)BS_ARG_GET(1, ##__VA_ARGS__); \
            _p.p[1] = (long)BS_ARG_GET(2, ##__VA_ARGS__); \
            _p.p[2] = (long)BS_ARG_GET(3, ##__VA_ARGS__); \
            _p.p[3] = (long)BS_ARG_GET(4, ##__VA_ARGS__); \
            _p.p[4] = (long)BS_ARG_GET(5, ##__VA_ARGS__); \
            if (_KLCHLP_BpfRun(_code, KLC_MAPFUNC_INSN_SIZE, &_p) >= 0) _ret = KLCHLP_Self(&_p.bpf_ret); \
        } \
        _ret; })


static __always_inline U64 KLCHLP_RunMapProg(void *ctx, void *map, unsigned int index)
{
    return _klc_helper(KLCHELP_RUN_MAP_PROG, (long)map, index, (long)ctx, 0);
}

static __always_inline void * _KLCHLP_GetMapByPath(char *path)
{
    unsigned long long map;
    map = _klc_helper(KLCHELP_GET_MAP_BY_PATH, (long)path, 0, 0, 0);
    if (map == KLC_RET_ERR) {
        return NULL;
    }

    return (void*)map;
}


#define KLCHLP_GetMapByPath(_path) ({ \
    char __path[] = _path; \
    void *ret; \
    ret = _KLCHLP_GetMapByPath(__path); \
    ret; \
})

#endif 
