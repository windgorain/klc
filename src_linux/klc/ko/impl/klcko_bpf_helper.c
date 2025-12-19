/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#include "klcko_impl.h"
#include "utl/bpf_helper_utl.h"
#include "utl/ulc_helper_id.h"
#include "utl/arch_utl.h"
#include "ko/ko_errcode.h"
#include "ko/ko_utl.h"
#include "klc/klc_kv_def.h"
#include "klc/klc_nl_def.h"
#include "klcko_bpf_helper.h"
#include "klcko_kv.h"
#include "klcko_setjmp.h"

static void * (*g_klcko_module_alloc)(int exe_size) = NULL;
static void (*g_klcko_module_free)(void *m) = NULL;

long _ulc_ret_0(void)
{
    return 0;
}

long _ulc_ret_1(void)
{
    return 1;
}

long _ulc_ret_2(void)
{
    return 2;
}

long _ulc_ret_65534(void)
{
    return 65534;
}

long _ulc_ret_n1(void)
{
    return -1;
}

static char * _ulc_ret_blank_str(void)
{
    return "";
}

static void _ulc_init_module_alloc(void)
{
    g_klcko_module_alloc = KLCKO_GetKV(KLC_KV_MODULE_ALLOC);
    g_klcko_module_free  = KLCKO_GetKV(KLC_KV_MODULE_FREE);
}

static void * ulc_sys_vmalloc(int size)
{
    return vmalloc(size); 
}

static void ulc_sys_vfree(void *m)
{
    vfree(m);
}

static void *ulc_sys_krealloc(const void *p, size_t new_size)
{
    return krealloc(p, new_size, GFP_ATOMIC);
}

static void * ulc_sys_kalloc(int size)
{
    return kmalloc(size, GFP_ATOMIC); 
}

static void ulc_sys_kfree(void *m)
{
    kfree(m);
}

static void * ulc_sys_module_alloc(int size)
{
    if (unlikely(! g_klcko_module_alloc)) {
        _ulc_init_module_alloc();
    }

    if (unlikely(! g_klcko_module_alloc)) {
        return NULL;
    }

    return g_klcko_module_alloc(size); 
}

static void ulc_sys_module_free(void *m)
{
    if (unlikely(! g_klcko_module_free)) {
        _ulc_init_module_alloc();
    }

    if (likely(g_klcko_module_free)) {
        g_klcko_module_free(m); 
    }
}

static unsigned long ulc_sys_copy_from_user(OUT void *to, void *from, unsigned long len)
{
    return copy_from_user(to, from ,len);
}

static unsigned long ulc_sys_copy_to_user(OUT void *to, void *from, unsigned long len)
{
    return copy_to_user(to, from ,len);
}

static void ulc_sys_usleep(U64 us)
{
    usleep_range(us, us);
}

static int ulc_sys_puts(const char *str)
{
    KO_Print("%s\n", str);
    return 0;
}

static int ulc_sys_printf(char *fmt, void *p1, void *p2, void *p3, void *p4)
{
    printk_ratelimited(fmt, p1, p2, p3, p4);
    return 0;
}

static int ulc_sys_printfx(char *fmt, U64 *d, int count)
{
    switch (count) {
        case 0: printk_ratelimited("%s",fmt); return 0;
        case 1: printk_ratelimited(fmt,d[0]); return 0;
        case 2: printk_ratelimited(fmt,d[0],d[1]); return 0;
        case 3: printk_ratelimited(fmt,d[0],d[1],d[2]); return 0;
        case 4: printk_ratelimited(fmt,d[0],d[1],d[2],d[3]); return 0;
        case 5: printk_ratelimited(fmt,d[0],d[1],d[2],d[3],d[4]); return 0;
        case 6: printk_ratelimited(fmt,d[0],d[1],d[2],d[3],d[4],d[5]); return 0;
        case 7: printk_ratelimited(fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6]); return 0;
        case 8: printk_ratelimited(fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7]); return 0;
        case 9: printk_ratelimited(fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8]); return 0;
        case 10:printk_ratelimited(fmt,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9]); return 0;
        default: return -1;
    }
}

static int ulc_sys_fprintf(void *fp, char *fmt, U64 *d, int count)
{
    if ((fp != (void*)1) && (fp != (void*)2)) {
        return -1;
    }

    return ulc_sys_printfx(fmt, d, count);
}

static size_t ulc_sys_fwrite(const void *ptr, size_t size, size_t nmemb, void *fp)
{
    if ((fp != (void*)1) && (fp != (void*)2)) {
        return -1;
    }

    printk_ratelimited("%s", (char*)ptr);

    return nmemb;
}

static int ulc_sys_fputs(const char *str, void *fp)
{
    if ((fp != (void*)1) && (fp != (void*)2)) {
        return -1;
    }

    printk_ratelimited("%s", str);

    return 0;
}

static int ulc_sys_fputc(int c, void *fp)
{
    if ((fp != (void*)1) && (fp != (void*)2)) {
        return -1;
    }

    printk_ratelimited("%c", c);

    return 0;
}


static void * g_bpf_runtime_ctrl = NULL; 

static void ulc_set_runtime(void *ptr)
{
    g_bpf_runtime_ctrl = ptr;
}

static void * ulc_get_runtime(void)
{
    return g_bpf_runtime_ctrl;
}


static void * g_bpf_helper_trusteeship[16];

static int ulc_set_trusteeship(unsigned int id, void *ptr)
{
    if (id >= ARRAY_SIZE(g_bpf_helper_trusteeship)) {
        return -1;
    }
    g_bpf_helper_trusteeship[id] = ptr;
    return 0;
}

static void * ulc_get_trusteeship(unsigned int id)
{
    if (id >= ARRAY_SIZE(g_bpf_helper_trusteeship)) {
        return NULL;
    }
    return g_bpf_helper_trusteeship[id];
}

static void ulc_do_nothing(void)
{
}

static int ulc_get_local_arch(void)
{
    return ARCH_LocalArch();
}

char * ulc_sys_env_name(void)
{
    return "linux-kernel";
}

static void * ulc_mmap_map(void *addr, U64 len, U64 flag, int fd, U64 off)
{
    int exe_size = round_up(len, PAGE_SIZE);

    
    return ulc_sys_module_alloc(exe_size);
}

static void ulc_mmap_munmap(void *m, U64 len)
{
    int exe_size = round_up(len, PAGE_SIZE);
    int (*func1)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_NX);
    int (*func2)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_RW); 

    if ((! func1) || (! func2)) {
        return;
    }

    func1(m, exe_size / PAGE_SIZE);
    func2(m, exe_size / PAGE_SIZE);

    ulc_sys_module_free(m);
}

static int ulc_mmap_mprotect(void *m, int size, U32 flag)
{
    int exe_size = round_up(size, PAGE_SIZE);
    int (*func1)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_RO); 
    int (*func2)(void *, int) = KLCKO_GetKV(KLC_KV_SET_MEM_X); 

    if ((! func1) || (! func2)) {
        return -1;
    }

    func1(m, exe_size/PAGE_SIZE);
    func2(m, exe_size/PAGE_SIZE);

    return 0;
}

static void ulc_sys_rcu_call(void *rcu, void *func)
{
    call_rcu(rcu, func);
}

static int ulc_sys_rcu_lock(void)
{
    rcu_read_lock();
    return 0;
}

static void ulc_sys_rcu_unlock(int state)
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


static int ulc_init_timer(void *timer_node, void *timeout_func, int node_size)
{
    if (node_size < sizeof(struct timer_list)) {
        return -1;
    }

    KO_SETUP_TIMER(timer_node, timeout_func, 0);

    return 0;
}

static int ulc_add_timer(void *timer_node, U32 ms)
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

static void ulc_del_timer(void *timer_node)
{
    del_timer(timer_node);
}

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

    [_(ULC_ID_PRINTF)] = ulc_sys_printf,
    [_(ULC_ID_PRINTFX)] = ulc_sys_printfx,
    [_(ULC_ID_PUTS)] = ulc_sys_puts,
    [_(ULC_ID_SPRINTF)] = sprintf,
    [_(ULC_ID_GETC)] = _ulc_ret_n1,
    [_(ULC_ID_UNGETC)] = _ulc_ret_n1,
    [_(ULC_ID_STRERROR)] = _ulc_ret_blank_str,
    [_(ULC_ID_FPRINTF)] = ulc_sys_fprintf,
    [_(ULC_ID_ACCESS)] = _ulc_ret_n1,
    [_(ULC_ID_FTELL)] = _ulc_ret_n1,
    [_(ULC_ID_FSEEK)] = _ulc_ret_n1,
    [_(ULC_ID_FOPEN)] = _ulc_ret_0,
    [_(ULC_ID_FREAD)] = _ulc_ret_n1,
    [_(ULC_ID_FCLOSE)] = _ulc_ret_n1,
    [_(ULC_ID_FGETS)] = _ulc_ret_n1,
    [_(ULC_ID_FWRITE)] = ulc_sys_fwrite,
    [_(ULC_ID_FREOPEN)] = _ulc_ret_0,
    [_(ULC_ID_FSCANF)] = _ulc_ret_n1,
    [_(ULC_ID_FERROR)] = _ulc_ret_n1,
    [_(ULC_ID_FEOF)] = _ulc_ret_n1,
    [_(ULC_ID_FPUTS)] = ulc_sys_fputs,
    [_(ULC_ID_FPUTC)] = ulc_sys_fputc,
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
    [_(ULC_ID_SETJMP)] = klcko_setjmp,
    [_(ULC_ID_LONGJMP)] = klcko_longjmp,

    [_(ULC_ID_INIT_TIMER)] = ulc_init_timer,
    [_(ULC_ID_ADD_TIMER)] = ulc_add_timer,
    [_(ULC_ID_DEL_TIMER)] = ulc_del_timer,

    [_(ULC_ID_MMAP_MAP)] = ulc_mmap_map,
    [_(ULC_ID_MMAP_UNMAP)] = ulc_mmap_munmap,
    [_(ULC_ID_MMAP_MPROTECT)] = ulc_mmap_mprotect,

    [_(ULC_ID_SET_TRUSTEESHIP)] = ulc_set_trusteeship,
    [_(ULC_ID_GET_TRUSTEESHIP)] = ulc_get_trusteeship,

    [_(ULC_ID_DO_NOTHING)] = ulc_do_nothing,
    [_(ULC_ID_LOCAL_ARCH)] = ulc_get_local_arch,

    [_(ULC_ID_SET_HELPER)] = ulc_set_helper,
    [_(ULC_ID_GET_HELPER)] = ulc_get_helper,
    [_(ULC_ID_GET_BASE_HELPER)] = ulc_get_base_helpers,
    [_(ULC_ID_GET_SYS_HELPER)] = ulc_get_sys_helpers,
    [_(ULC_ID_GET_USER_HELPER)] = ulc_get_user_helpers,
    [_(ULC_ID_ENV_NAME)] = ulc_sys_env_name,

    [_(ULC_ID_SET_RUNTIME)] = ulc_set_runtime,
    [_(ULC_ID_GET_RUNTIME)] = ulc_get_runtime,
};
static const void * g_bpf_user_helpers[BPF_USER_HELPER_COUNT];

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

