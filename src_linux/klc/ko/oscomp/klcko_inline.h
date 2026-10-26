/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-24
* Description: 
******************************************************************************/
#ifndef _KLCKO_INLINE_H_
#define _KLCKO_INLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#undef inline
#define inline

#undef __always_inline 
#define __always_inline

#define static

#undef WARN_ON
#define WARN_ON(condition...) ({(void)(condition);0;})

#undef WARN_ON_ONCE
#define WARN_ON_ONCE(condition) WARN_ON(condition)

#undef WARN
#define WARN(condition...) WARN_ON(condition)

#undef WARN_ONCE
#define WARN_ONCE(condition...) WARN_ON(condition)

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-value"
#endif


#ifdef __cplusplus
}
#endif
#endif 
