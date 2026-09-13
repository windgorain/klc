/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-13
* Description: 
******************************************************************************/
#ifndef _LUAXDP_DEF_H_
#define _LUAXDP_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned int valid: 1; 
    unsigned char lua_code[4096];
}LUAXDP_CODE_S;

#ifdef __cplusplus
}
#endif
#endif 
