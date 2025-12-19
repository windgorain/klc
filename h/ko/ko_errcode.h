/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _KO_ERRCODE_H_
#define _KO_ERRCODE_H_
#ifdef __cplusplus
extern "C"
{
#endif

#define KO_ERR_DEF \
    _(KO_ERR_FAIL, "failed") \
    _(KO_ERR_BAD_PARAM, "bad pararm") \
    _(KO_ERR_NO_SUCH, "no such") \
    _(KO_ERR_EXIST, "already exist") \
    _(KO_ERR_NO_MEMORY, "no memroy") \
    _(KO_ERR_NOT_READY, "not ready") \

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
