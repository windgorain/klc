/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "klcko_lib.h"
#include "klcko_pt.h"
#include "ko/ko_utl.h"
#include "utl/mdef_utl.h"

#define KLCHELP_CLASS(cmd) (((cmd) >> 16) & 0xff)

static int g_klc_base_enabled = 0;
static PF_KLC_HELPER g_klc_base_funcs[KLCKO_BASE_FUNC_NUM];
static PF_KLC_HELPER g_klc_base_agent_func[1000];

struct bpf_verifier_ops g_klc_hook_ops;

#define MDEF_DEF_FUNC(_str, _id) U64 klc_base_agent##_str(U64 p1, U64 p2, U64 p3, U64 p4, U64 p5) { \
    PF_KLC_HELPER f = g_klc_base_agent_func[_id]; \
    if (! f) { return -1LL; } \
    return f(p1, p2, p3, p4, p5); \
}

MDEF_1000_FUNCS;

#undef MDEF_DEF_FUNC

static inline u64 _klc_base_call(u64 cmd, u64 p2, u64 p3, u64 p4, u64 p5)
{
    PF_KLC_HELPER func;
    u64 ret;
    int class = KLCHELP_CLASS(cmd);

    func = g_klc_base_funcs[class];
    if (likely(func)) {
        ret = func(cmd, p2, p3, p4, p5);
    } else {
        ret = KLC_RET_ERR;
        KO_Print("Can't find class %d \n", class);
    }

    return ret;
}


u64 bpf_map_update_ele(u64 cmd, u64 p2, u64 p3, u64 p4, u64 p5)
{
    if (! g_klc_base_enabled) {
        return -1LL;
    }

    return _klc_base_call(cmd, p2, p3, p4, p5);
}

int KlcKo_SetBaseFunc(U32 index, void *func)
{
    if (index >= ARRAY_SIZE(g_klc_base_funcs)) {
        return -1;
    }

    g_klc_base_funcs[index] = func;

    return 0;
}
EXPORT_SYMBOL(KlcKo_SetBaseFunc);

void KlcKo_BaseEnable(int enable)
{
    g_klc_base_enabled = enable;
}
EXPORT_SYMBOL(KlcKo_BaseEnable);

void klc_base_module_use(int inc)
{
    static atomic64_t used = {0};
    u64 old;

    inc = !!inc;

    if (inc) {
        old = atomic64_fetch_add(1, &used);
        if (old == 0) {
            __module_get(THIS_MODULE);
        }
    } else {
        old = atomic64_fetch_add(-1, &used);
        if (old == 1) {
            module_put(THIS_MODULE);
        }
    }
}
EXPORT_SYMBOL(klc_base_module_use);

static int __init _klc_base_init(void)
{
    return 0;
}

static void __exit _klc_base_exit(void)
{
    synchronize_rcu();
    rcu_barrier(); 
}

module_init(_klc_base_init)
module_exit(_klc_base_exit)
MODULE_LICENSE("GPL");

