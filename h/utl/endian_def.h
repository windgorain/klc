/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-11
* Description: 
******************************************************************************/
#ifndef _ENDIAN_DEF_H_
#define _ENDIAN_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define BS_BIG_ENDIAN 1
#define BS_LITTLE_ENDIAN 0
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BS_BIG_ENDIAN 0
#define BS_LITTLE_ENDIAN 1
#else
#error "Error"
#endif

#ifdef __cplusplus
}
#endif
#endif 
