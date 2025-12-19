/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_TC_DEF_H_
#define _MYBPF_TC_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TC_ACT_OK
#define TC_ACT_UNSPEC	(-1)
#define TC_ACT_OK		0
#define TC_ACT_RECLASSIFY	1
#define TC_ACT_SHOT		2
#define TC_ACT_PIPE		3
#define TC_ACT_STOLEN		4
#define TC_ACT_QUEUED		5
#define TC_ACT_REPEAT		6
#define TC_ACT_REDIRECT		7
#define TC_ACT_TRAP		8
#endif

typedef struct {
    void *data;
    void *data_end;
    void *data_meta;
}MYBPF_TC_S;

#ifdef __cplusplus
}
#endif
#endif 
