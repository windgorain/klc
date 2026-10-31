/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _KO_ERRCODE_H_
#define _KO_ERRCODE_H_
#ifdef __cplusplus
extern "C"
{
#endif

#define KO_ERR_DEF \
    _(KO_ERR_FAIL, "Failed") \
    _(KO_ERR_BAD_PARAM, "Bad pararm") \
    _(KO_ERR_NO_SUCH, "No such") \
    _(KO_ERR_EXIST, "Already exist") \
    _(KO_ERR_NO_MEMORY, "No memroy") \
    _(KO_ERR_NOT_READY, "Not ready") \
    _(KO_ERR_NEED_ROOT, "Permission denied") \

enum {
    KO_ERR_BASE = -2000,
#define _(a,b) a,
    KO_ERR_DEF
#undef _
    KO_ERR_MAX,
};

#ifdef __cplusplus
}
#endif
#endif 
