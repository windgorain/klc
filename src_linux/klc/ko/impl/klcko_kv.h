/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _KLCKO_KV_H
#define _KLCKO_KV_H

#ifdef __cplusplus
extern "C" {
#endif

void * KLCKO_GetKV(U32 id);
int KlcKoKv_Init(void);
void KlcKoKv_Fini(void);

#ifdef __cplusplus
}
#endif
#endif 
