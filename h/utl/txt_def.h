/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-26
* Description: 
******************************************************************************/
#ifndef _TXT_DEF_H_
#define _TXT_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif


#define TXT_STRING_ARRAY(_x, _v) _x[] = _v; _x[sizeof(_v) - 1] = '\0'

#ifdef __cplusplus
}
#endif
#endif 
