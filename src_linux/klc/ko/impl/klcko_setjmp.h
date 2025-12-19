/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _KLCKO_SETJMP_H_
#define _KLCKO_SETJMP_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    U8 data[328];
}KLCKO_JMP_S;

int klcko_setjmp(KLCKO_JMP_S *env);
void klcko_longjmp(KLCKO_JMP_S *env, int val);

#ifdef __cplusplus
}
#endif
#endif 
