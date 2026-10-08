/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "klcko_impl.h"
#include "utl/bpf_helper_utl.h"
#include "utl/ulc_helper_id.h"
#include "utl/cpu_def.h"
#include "utl/arch_utl.h"
#include "ko/ko_errcode.h"
#include "ko/ko_utl.h"
#include "klc/klc_kv_def.h"
#include "klc/klc_nl_def.h"
#include "klcko_bpf_helper.h"
#include "klcko_kv.h"
#include "klcko_setjmp.h"

static const void * g_bpf_base_helpers[];
static const void * g_bpf_sys_helpers[];
static const void * g_bpf_user_helpers[];

static void * (*g_klcko_module_alloc)(int size) = NULL;
static void * (*g_klcko_module_alloc_ext)(int type, int size) = NULL;
static void (*g_klcko_module_free)(void *m) = NULL;

S64 _ulc_ret_0(void);
S64 _ulc_ret_1(void);
S64 _ulc_ret_2(void);
S64 _ulc_ret_65534(void);
S64 _ulc_ret_n1(void);

S64 _ulc_ret_0(void)
{
    return 0;
}

S64 _ulc_ret_1(void)
{
    return 1;
}

S64 _ulc_ret_2(void)
{
    return 2;
}

S64 _ulc_ret_65534(void)
{
    return 65534;
}

S64 _ulc_ret_n1(void)
{
    return -1;
}

static inline char * _ulc_ret_blank_str(void)
{
    return "";
}

static void _ulc_init_module_alloc(void)
{
    g_klcko_module_alloc = KLCKO_GetKV(KLC_KV_MODULE_ALLOC);
    g_klcko_module_free = KLCKO_GetKV(KLC_KV_MODULE_FREE);
    if (! g_klcko_module_alloc) {
        g_klcko_module_alloc = KLCKO_GetKV(KLC_KV_JIT_ALLOC);
        g_klcko_module_free = KLCKO_GetKV(KLC_KV_JIT_FREE);
    }

    if (! g_klcko_module_alloc) {
        g_klcko_module_alloc_ext = KLCKO_GetKV(KLC_KV_EXECMEM_ALLOC);
    }
}

static inline void * ulc_sys_vmalloc(int size)
{
    return vmalloc(size); 
}

static inline void ulc_sys_vfree(void *m)
{
    vfree(m);
}

static inline void *ulc_sys_krealloc(const void *p, size_t new_size)
{
    return krealloc(p, new_size, GFP_ATOMIC);
}

static inline void * ulc_sys_kalloc(int size)
{
    return kmalloc(size, GFP_ATOMIC); 
}

static inline void ulc_sys_kfree(void *m)
{
    kfree(m);
}

static inline void * ulc_sys_module_alloc(int size)
{
    if (unlikely(! g_klcko_module_free)) {
        _ulc_init_module_alloc();
    }

    if (unlikely(! g_klcko_module_free)) {
        return NULL;
    }

    if (g_klcko_module_alloc) {
        return g_klcko_module_alloc(size); 
    }

    if (g_klcko_module_alloc_ext) {
        return g_klcko_module_alloc_ext(3, size); 
    }

    return NULL;
}

static inline void ulc_sys_module_free(void *m)
{
    if (unlikely(! g_klcko_module_free)) {
        _ulc_init_module_alloc();
    }

    if (likely(g_klcko_module_free)) {
        g_klcko_module_free(m); 
    }
}

static inline unsigned long ulc_sys_copy_from_user(OUT void *to, void *from, unsigned long len)
{
    return copy_from_user(to, from ,len);
}

static inline unsigned long ulc_sys_copy_to_user(OUT void *to, void *from, unsigned long len)
{
    return copy_to_user(to, from ,len);
}

#if 0
static int ulc_sys_printf(U64 fmt, U64 p1, U64 p2, U64 p3, U64 p4)
{
    char info[1024];
    U64 param[4];

    long (*_my_bpf_snprintf)(U64 str, U64 str_size, U64 fmt, U64 data, U64 data_len) = g_bpf_base_helpers[165];

    if (! _my_bpf_snprintf) {
        return 0;
    }

    param[0] = p1;
    param[1] = p2;
    param[2] = p3;
    param[3] = p4;

    _my_bpf_snprintf((U64)(LONG)(void*)info, (U64)sizeof(info), fmt, (U64)(LONG)(void*)param, 32LL);

    printk("%s\n", info);

    return 0;
}
#endif

static inline U32 _ulc_sys_atomic32(U8 op, U32 *ptr, U32 val, U32 expected)
{
    switch (op) {
        case 0: return __sync_fetch_and_add(ptr, val);
        case 1: return __sync_fetch_and_and(ptr, val);
        case 2: return __sync_fetch_and_or(ptr, val);
        case 3: return __sync_fetch_and_xor(ptr, val);
        case 4: return __sync_val_compare_and_swap(ptr, *ptr, val);
        case 5: return __sync_val_compare_and_swap(ptr, expected, val);
    }
    printk("Not support op:%u\r\n", op);
    return 0;
}

static inline U64 _ulc_sys_atomic64(U8 op, U64 *ptr, U64 val, U64 expected)
{
    switch (op) {
        case 0: return __sync_fetch_and_add(ptr, val);
        case 1: return __sync_fetch_and_and(ptr, val);
        case 2: return __sync_fetch_and_or(ptr, val);
        case 3: return __sync_fetch_and_xor(ptr, val);
        case 4: return __sync_val_compare_and_swap(ptr, *ptr, val);
        case 5: return __sync_val_compare_and_swap(ptr, expected, val);
    }
    printk("Not support op:%u\r\n", op);
    return 0;
}




static U64 ulc_sys_atomic(U32 opt, void *ptr, U64 val, U64 expected)
{
    U8 op = opt & 0xf;
    U8 isdw = (opt & 0x80) ? 1 : 0;

    if (isdw) {
        return _ulc_sys_atomic64(op, ptr, val, expected);
    } else {
        return _ulc_sys_atomic32(op, ptr, val, expected);
    }
}

static void ulc_sys_enter_func(U32 r0, U32 r1)
{
    printk("r0=0x%08x r1=0x%08x\n", r0, r1);
}


static U64 ulc_sys_udiv(U64 p1, U64 p2)
{
    if (p2 == 0) {
        return 0;
    }
    return div_u64(p1, p2);
}


static U64 ulc_sys_umod(U64 p1, U64 p2)
{
    u32 rem;

    if (p2 == 0) {
        return 0;
    }

    div_u64_rem(p1, p2, &rem);

    return rem;
}

static S64 ulc_sys_ptr_size(void)
{
    return sizeof(void*);
}

static void ulc_sys_usleep(U64 us)
{
    usleep_range(us, us);
}

static inline int ulc_sys_puts(const char *str)
{
    KO_Print("%s\n", str);
    return 0;
}

static void * g_bpf_runtime_ctrl = NULL; 

static inline void ulc_set_runtime(void *ptr)
{
    g_bpf_runtime_ctrl = ptr;
}

static void * ulc_get_runtime(void)
{
    return g_bpf_runtime_ctrl;
}

static void ulc_do_nothing(void)
{
}

static S64 ulc_get_local_arch(void)
{
    return ARCH_LocalArch();
}

static inline void * ulc_mmap_map(void *addr, U64 len, U64 flag, int fd, U64 off)
{
    int exe_size = round_up(len, PAGE_SIZE);

    
    void *m = ulc_sys_module_alloc(exe_size);
    if (! m) {
        return ((void *) -1);
    }
    return m;
}

static inline int ulc_mmap_unmap(void *m, U64 len)
{
    int exe_size = round_up(len, PAGE_SIZE);
    int (*func1)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_NX);
    int (*func2)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_RW); 

    if ((! m) || (m == (void*)-1)) {
        return 0;
    }

    if ((! func1) || (! func2)) {
        return 0;
    }

    func1(m, exe_size / PAGE_SIZE);
    func2(m, exe_size / PAGE_SIZE);

    ulc_sys_module_free(m);

    return 0;
}

static inline int ulc_mmap_mprotect(void *m, int size, U32 flag)
{
    int ret;
    struct vm_struct *vm;

    int exe_size = round_up(size, PAGE_SIZE);
    int (*func1)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_RO); 
    int (*func2)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_X); 
    void * (*func3)(void *) = KLCKO_GetKV(KLC_KV_FIND_VM_AREA);

    if ((! func1) || (! func2) || (! func3)) {
        return -1;
    }

    vm = func3(m);
    if (vm) {
		vm->flags |= VM_FLUSH_RESET_PERMS;
    }

    ret = func1(m, exe_size/PAGE_SIZE);
    ret |= func2(m, exe_size/PAGE_SIZE);

    return ret;
}

static inline void ulc_sys_rcu_call(void *rcu, void *func)
{
    call_rcu(rcu, func);
}

static int ulc_sys_rcu_lock(void)
{
    rcu_read_lock();
    return 0;
}

static void ulc_sys_rcu_unlock(void)
{
    rcu_read_unlock();
}

static void ulc_sys_rcu_sync(void)
{
    synchronize_rcu();
}

static void ulc_sys_rcu_barrier(void)
{
    rcu_barrier();
}

static inline int ulc_init_timer(void *timer_node, void *timeout_func, int node_size)
{
    if (node_size < sizeof(struct timer_list)) {
        return -1;
    }

    KO_SETUP_TIMER(timer_node, timeout_func, 0);

    return 0;
}

static inline int ulc_add_timer(void *timer_node, U32 ms)
{
    unsigned int t;
    struct timer_list *timer = timer_node;

    if (HZ < 1000) {
        t = round_up(ms, 1000/HZ);
        t = (t * HZ) / 1000;
    } else {
        t = (ms * HZ) / 1000;
    }

    timer->expires = jiffies + t;
    add_timer(timer);

    return 0;
}

static inline void ulc_del_timer(void *timer_node)
{
    
    timer_delete(timer_node);
}

const void ** ulc_get_base_helpers(void)
{
    return g_bpf_base_helpers;
}

const void ** ulc_get_sys_helpers(void)
{
    return g_bpf_sys_helpers;
}

const void ** ulc_get_user_helpers(void)
{
    return g_bpf_user_helpers;
}

int ulc_set_helper(U32 id, void *func)
{
    if (func == NULL) {
        func = _ulc_ret_0;
    } else if (func == (void*)(long)1) {
        func = _ulc_ret_1;
    } else if (func == (void*)(long)-1) {
        func = _ulc_ret_n1;
    }

    if (id < BPF_BASE_HELPER_END) {
        g_bpf_base_helpers[id] = func;
    } else if ((id >= BPF_SYS_HELPER_START) && (id < BPF_SYS_HELPER_END)) {
        g_bpf_sys_helpers[id - BPF_SYS_HELPER_START] = func;
    } else if ((BPF_USER_HELPER_START <= id) && (id < BPF_USER_HELPER_END)) {
        g_bpf_user_helpers[id - BPF_USER_HELPER_START] = func;
    } else {
        return -1;
    }

    return 0;
}

void * ulc_get_helper(unsigned int id, const void **tmp_helpers)
{
    if (id < BPF_BASE_HELPER_END) {
        return (void*)g_bpf_base_helpers[id];
    } else if ((id >= BPF_SYS_HELPER_START) && (id < BPF_SYS_HELPER_END)) {
        return (void*)g_bpf_sys_helpers[id - BPF_SYS_HELPER_START];
    } else if ((id >= BPF_USER_HELPER_START) && (id < BPF_USER_HELPER_END)) {
        return (void*)g_bpf_user_helpers[id - BPF_USER_HELPER_START];
    } else if ((id >= BPF_TMP_HELPER_START) && (id < BPF_TMP_HELPER_END) && (tmp_helpers)) {
        int idx = id - BPF_TMP_HELPER_START;
        if ((idx <= 0) || (idx >= *(U32*)tmp_helpers)) { 
            return NULL;
        }
        return (void*)tmp_helpers[idx];
    }

    return NULL;
}

static int _klcko_helper_set(KLC_KV_SET_NL_S *d)
{
    U32 id = d->id;
    void *func = (void*)(long)d->value;
    return ulc_set_helper(id, func);
}

#if PLATFORM_32BIT 

static S64 __ulc_ret_blank_str(void)
{
    return (LONG)_ulc_ret_blank_str();
}
#define _ulc_ret_blank_str __ulc_ret_blank_str

static S64 _ulc_sys_vmalloc(U64 p1)
{
    return (LONG)ulc_sys_vmalloc(p1);
}
#define ulc_sys_vmalloc _ulc_sys_vmalloc

static void _ulc_sys_vfree(U64 p1)
{
    ulc_sys_vfree((void*)(LONG)p1);
}
#define ulc_sys_vfree _ulc_sys_vfree

static S64 _ulc_sys_krealloc(U64 p1, U64 p2)
{
    return (LONG)ulc_sys_krealloc((void*)(LONG)p1, p2);
}
#define ulc_sys_krealloc _ulc_sys_krealloc

static S64 _ulc_sys_kalloc(U64 p1)
{
    return (LONG)ulc_sys_kalloc(p1);
}
#define ulc_sys_kalloc _ulc_sys_kalloc

static void _ulc_sys_kfree(U64 p1)
{
    ulc_sys_kfree((void*)(LONG)p1);
}
#define ulc_sys_kfree _ulc_sys_kfree

static S64 _ulc_sys_module_alloc(U64 p1)
{
    return (LONG)ulc_sys_module_alloc(p1);
}
#define ulc_sys_module_alloc _ulc_sys_module_alloc

static void _ulc_sys_module_free(U64 p1)
{
    ulc_sys_module_free((void*)(LONG)p1);
}
#define ulc_sys_module_free _ulc_sys_module_free

static U64 _ulc_sys_copy_from_user(U64 p1, U64 p2, U64 p3)
{
    return ulc_sys_copy_from_user((void*)(LONG)p1, (void*)(LONG)p2, p3);
}
#define ulc_sys_copy_from_user _ulc_sys_copy_from_user

static U64 _ulc_sys_copy_to_user(U64 p1, U64 p2, U64 p3)
{
    return ulc_sys_copy_to_user((void*)(LONG)p1, (void*)(LONG)p2, p3);
}
#define ulc_sys_copy_to_user _ulc_sys_copy_to_user

static S64 _ulc_sys_puts(U64 p1)
{
    return ulc_sys_puts((void*)(LONG)p1);
}
#define ulc_sys_puts _ulc_sys_puts

static void _ulc_set_runtime(U64 p1)
{
    ulc_set_runtime((void*)(LONG)p1);
}
#define ulc_set_runtime _ulc_set_runtime

static S64 _ulc_get_runtime(void)
{
    return (LONG)ulc_get_runtime();
}
#define ulc_get_runtime _ulc_get_runtime

static S64 _ulc_mmap_map(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5)
{
    return (LONG)ulc_mmap_map((void*)(LONG)p1, p2, p3, p4, p5);
}
#define ulc_mmap_map _ulc_mmap_map

static S64 _ulc_mmap_unmap(U64 p1, U64 p2)
{
    return ulc_mmap_unmap((void*)(LONG)p1, p2);
}
#define ulc_mmap_unmap _ulc_mmap_unmap

static S64 _ulc_mmap_mprotect(U64 p1, U64 p2, U64 p3)
{
    return ulc_mmap_mprotect((void*)(LONG)p1, p2, p3);
}
#define ulc_mmap_mprotect _ulc_mmap_mprotect

static void _ulc_sys_rcu_call(U64 p1, U64 p2)
{
    ulc_sys_rcu_call((void*)(LONG)p1, (void*)(LONG)p2);
}
#define ulc_sys_rcu_call _ulc_sys_rcu_call

static S64 _ulc_init_timer(U64 p1, U64 p2, U64 p3)
{
    return ulc_init_timer((void*)(LONG)p1, (void*)(LONG)p2, p3);
}
#define ulc_init_timer _ulc_init_timer

static S64 _ulc_add_timer(U64 p1, U64 p2)
{
    return ulc_add_timer((void*)(LONG)p1, p2);
}
#define ulc_add_timer _ulc_add_timer

static void _ulc_del_timer(U64 p1)
{
    ulc_del_timer((void*)(LONG)p1);
}
#define ulc_del_timer _ulc_del_timer

static S64 _ulc_get_helper(U64 p1, U64 p2)
{
    return (LONG)ulc_get_helper(p1, (void*)(LONG)p2);
}
#define ulc_get_helper _ulc_get_helper

static S64 _ulc_set_helper(U64 p1, U64 p2)
{
    return ulc_set_helper(p1, (void*)(LONG)p2);
}
#define ulc_set_helper _ulc_set_helper

static S64 _ulc_get_base_helpers(void)
{
    return (LONG)ulc_get_base_helpers();
}
#define ulc_get_base_helpers _ulc_get_base_helpers

static S64 _ulc_get_sys_helpers(void)
{
    return (LONG)ulc_get_sys_helpers();
}
#define ulc_get_sys_helpers _ulc_get_sys_helpers

static S64 _ulc_get_user_helpers(void)
{
    return (LONG)ulc_get_user_helpers();
}
#define ulc_get_user_helpers _ulc_get_user_helpers

#endif

static const void * g_bpf_base_helpers[BPF_BASE_HELPER_COUNT];

#undef _
#define _(x) ((x) - 1000000)
static const void * g_bpf_sys_helpers[BPF_SYS_HELPER_COUNT] = {
    [0] = NULL, 
    [_(ULC_ID_MALLOC)] = ulc_sys_kalloc,
    [_(ULC_ID_FREE)] = ulc_sys_kfree,
    [_(ULC_ID_VMALLOC)] = ulc_sys_vmalloc,
    [_(ULC_ID_VFREE)] = ulc_sys_vfree,
    [_(ULC_ID_REALLOC)] = ulc_sys_krealloc,
    [_(ULC_ID_MODULE_ALLOC)] = ulc_sys_module_alloc,
    [_(ULC_ID_MODULE_FREE)] = ulc_sys_module_free,
    [_(ULC_ID_COPY_FROM_USER)] = ulc_sys_copy_from_user,
    [_(ULC_ID_COPY_TO_USER)] = ulc_sys_copy_to_user,
    [_(ULC_ID_ATOMIC)] = ulc_sys_atomic,
    [_(ULC_ID_UDIV)] = ulc_sys_udiv,
    [_(ULC_ID_UMOD)] = ulc_sys_umod,
    [_(ULC_ID_PTR_SIZE)] = ulc_sys_ptr_size,
    [_(ULC_ID_ENTER_FUNC)] = ulc_sys_enter_func,

    [_(ULC_ID_PRINTF)] = _ulc_ret_0,
    [_(ULC_ID_PUTS)] = ulc_sys_puts,
    [_(ULC_ID_GETC)] = _ulc_ret_n1,
    [_(ULC_ID_UNGETC)] = _ulc_ret_n1,
    [_(ULC_ID_STRERROR)] = _ulc_ret_blank_str,
    [_(ULC_ID_ACCESS)] = _ulc_ret_n1,
    [_(ULC_ID_FTELL)] = _ulc_ret_n1,
    [_(ULC_ID_FSEEK)] = _ulc_ret_n1,
    [_(ULC_ID_FOPEN)] = _ulc_ret_0,
    [_(ULC_ID_FREAD)] = _ulc_ret_n1,
    [_(ULC_ID_FCLOSE)] = _ulc_ret_n1,
    [_(ULC_ID_FGETS)] = _ulc_ret_n1,
    [_(ULC_ID_FREOPEN)] = _ulc_ret_0,
    [_(ULC_ID_FSCANF)] = _ulc_ret_n1,
    [_(ULC_ID_FERROR)] = _ulc_ret_n1,
    [_(ULC_ID_FEOF)] = _ulc_ret_n1,
    [_(ULC_ID_FFLUSH)] = _ulc_ret_n1,
    [_(ULC_ID_SETVBUF)] = _ulc_ret_n1,
    [_(ULC_ID_CLEARERR)] = _ulc_ret_n1,
    [_(ULC_ID_TMPFILE)] = _ulc_ret_0,
    [_(ULC_ID_TMPNAM)] = _ulc_ret_0,

    [_(ULC_ID_LOCALECONV)] = _ulc_ret_0,
    [_(ULC_ID_SETLOCALE)] = _ulc_ret_0,
    [_(ULC_ID_USLEEP)] = ulc_sys_usleep,
    [_(ULC_ID_GETENV)] = _ulc_ret_0,
    [_(ULC_ID_CLOCK)] = ktime_get,
    [_(ULC_ID_TIME)] = ktime_get_seconds,
    [_(ULC_ID_GMTIME)] = _ulc_ret_0,
    [_(ULC_ID_LOCALTIME)] = _ulc_ret_0,
    [_(ULC_ID_STRFTIME)] = _ulc_ret_0,
    [_(ULC_ID_MKTIME)] = _ulc_ret_n1,
    [_(ULC_ID_SYSTEM)] = _ulc_ret_n1,
    [_(ULC_ID_REMOVE)] = _ulc_ret_n1,
    [_(ULC_ID_RENAME)] = _ulc_ret_n1,
    [_(ULC_ID_SIGNAL)] = _ulc_ret_n1,

    [_(ULC_ID_RCU_CALL)] = ulc_sys_rcu_call,
    [_(ULC_ID_RCU_LOCK)] = ulc_sys_rcu_lock,
    [_(ULC_ID_RCU_UNLOCK)] = ulc_sys_rcu_unlock,
    [_(ULC_ID_RCU_SYNC)] = ulc_sys_rcu_sync,
    [_(ULC_ID_RCU_BARRIER)] = ulc_sys_rcu_barrier,

    [_(ULC_ID_ERRNO)] = _ulc_ret_n1,
    [_(ULC_ID_SET_ERRNO)] = _ulc_ret_0,

#if defined(__aarch64__) || defined(__x86_64__) || defined(__arm__)
    [_(ULC_ID_SETJMP)] = klcko_setjmp,
    [_(ULC_ID_LONGJMP)] = klcko_longjmp,
#endif

    [_(ULC_ID_INIT_TIMER)] = ulc_init_timer,
    [_(ULC_ID_ADD_TIMER)] = ulc_add_timer,
    [_(ULC_ID_DEL_TIMER)] = ulc_del_timer,

    [_(ULC_ID_MMAP_MAP)] = ulc_mmap_map,
    [_(ULC_ID_MMAP_UNMAP)] = ulc_mmap_unmap,
    [_(ULC_ID_MMAP_MPROTECT)] = ulc_mmap_mprotect,

    [_(ULC_ID_DO_NOTHING)] = ulc_do_nothing,
    [_(ULC_ID_LOCAL_ARCH)] = ulc_get_local_arch,

    [_(ULC_ID_SET_HELPER)] = ulc_set_helper,
    [_(ULC_ID_GET_HELPER)] = ulc_get_helper,
    [_(ULC_ID_GET_BASE_HELPER)] = ulc_get_base_helpers,
    [_(ULC_ID_GET_SYS_HELPER)] = ulc_get_sys_helpers,
    [_(ULC_ID_GET_USER_HELPER)] = ulc_get_user_helpers,

    [_(ULC_ID_SET_RUNTIME)] = ulc_set_runtime,
    [_(ULC_ID_GET_RUNTIME)] = ulc_get_runtime,
};
static const void * g_bpf_user_helpers[BPF_USER_HELPER_COUNT];

static int _klcko_helper_nl_do(int cmd, void *data, int data_len, OUT void *reply, int reply_size, OUT int *reply_len)
{
    if (cmd == KLC_NL_HELPER_SET) {
        return _klcko_helper_set(data);
    }

    return KO_ERR_BAD_PARAM;
}

int KlcKoHelper_Init(void)
{
    return KlcKoNl_Reg(KLC_NL_TYPE_HELPER, _klcko_helper_nl_do);
}

void KlcKoHelper_Fini(void)
{
    KlcKoNl_Reg(KLC_NL_TYPE_HELPER, NULL);
}

