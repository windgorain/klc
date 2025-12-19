/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#ifndef _KLCKO_BARE_H
#define _KLCKO_BARE_H

#include "utl/mybpf_def.h"
#include "utl/mybpf_bare_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

int MYBPF_LoadBare(void *data, int len, const void **tmp_helpers, OUT MYBPF_BARE_S *bare);
U64 MYBPF_RunBareMain(MYBPF_BARE_S *bare, MYBPF_PARAM_S *p);
void MYBPF_UnloadBare(MYBPF_BARE_S *bare);

#ifdef __cplusplus
}
#endif
#endif 
