/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#include "klcko_impl.h"
#include "ko/ko_errcode.h"
#include "klc/klc_kv_def.h"
#include "klc/klc_nl_def.h"

static U64 g_klcko_kv[KLC_KV_MAX];

static int _klcko_set_kv(U32 id, U64 value)
{
    if (id >= KLC_KV_MAX) {
        return -1;
    }

    g_klcko_kv[id] = value;

    return 0;
}

U64 KLCKO_GetKV(U32 id)
{
    if (id >= KLC_KV_MAX) {
        return 0;
    }

    return g_klcko_kv[id];
}

static int _klcko_kv_set(KLC_KV_SET_NL_S *d)
{
    return _klcko_set_kv(d->id, d->value);
}

static int _klcko_kv_nl_do(int cmd, void *data, int data_len, OUT void *reply, int reply_size, OUT int *reply_len)
{
    if (cmd == KLC_NL_KV_SET) {
        return _klcko_kv_set(data);
    }

    return KO_ERR_BAD_PARAM;
}

int KlcKoKv_Init(void)
{
    return KlcKoNl_Reg(KLC_NL_TYPE_KV, _klcko_kv_nl_do);
}

void KlcKoKv_Fini(void)
{
    KlcKoNl_Reg(KLC_NL_TYPE_KV, NULL);
}

