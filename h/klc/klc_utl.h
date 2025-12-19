/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#ifndef _KLC_UTL_H
#define _KLC_UTL_H

#include "utl/mybpf_utl.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum {
    KLC_KHELPER_MIOCTL_NAMEMAP_ADD = 0, 
    KLC_KHELPER_MIOCTL_NAMEMAP_DEL,     
    KLC_KHELPER_MIOCTL_NAMEMAP_GET,
};

typedef struct {
    char name[MYBPF_NAME_SIZE];
    int fd;
}KLC_KHELPER_NAMEMAP_S;


#ifdef __cplusplus
}
#endif
#endif
