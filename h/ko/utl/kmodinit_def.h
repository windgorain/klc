/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#ifndef _KMODINIT_DEF_H
#define _KMODINIT_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*PF_KUTL_MODINIT_FUNC)(void);
typedef void (*PF_KUTL_MODFINI_FUNC)(void);

typedef struct {
    char *name;
    PF_KUTL_MODINIT_FUNC init;
    PF_KUTL_MODFINI_FUNC fini;
    int inited;
}KUTL_MODINIT_S;

#ifdef __cplusplus
}
#endif
#endif 
