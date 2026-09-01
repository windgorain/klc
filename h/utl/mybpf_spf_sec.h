/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_SPF_SEC_H_
#define _MYBPF_SPF_SEC_H_

#include "spf/spf_evob_ev.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPF_SEC_EVENT ".spf.event"
#define SPF_SEC_CMD_PX ".spf.cmd/" 
#define SPF_SEC_DEPENDS ".spf.depends"
#define SPF_SEC_DATA_INIT ".spf.data.init"
#define SPF_SEC_NAMEFUNC_PX ".spf.namefunc/"
#define SPF_SEC_IDFUNC_PX ".spf.idfunc/"
#define SPF_SEC_EVOB_PX ".spf.evob/"


#define SPF_SEC_BPF_MAPS "maps"
#define SPF_MAPS_PATH "/sys/fs/bpf/klc/maps/"
#define BPF_MAPS_PATH "/sys/fs/bpf/"


#define SPF_SEC_MAP ".spf.maps" 

#define SPF_SEC_MODULE_IOCTL ".spf.module_ioctl"

#define SPF_EVOB_PRI_HIGH   1000000
#define SPF_EVOB_PRI_NORMAL 2000000
#define SPF_EVOB_PRI_LOW    3000000

#define _SEC_EVOB(_event, _pri) SEC(SPF_SEC_EVOB_PX #_event "/" #_pri)
#define SEC_EVOB(_event, _pri) _SEC_EVOB(_event, _pri) 

#define _SEC_ID_FUNC(_id) SEC(SPF_SEC_IDFUNC_PX #_id)
#define SEC_ID_FUNC(_id) _SEC_ID_FUNC(_id) 

#define _SEC_NAME_FUNC(_name) SEC(SPF_SEC_NAMEFUNC_PX _name)
#define SEC_NAME_FUNC(_name) _SEC_NAME_FUNC(_name)

enum {
    SPF_EVENT_SET_SELF = 0, 
    SPF_EVENT_INIT, 
    SPF_EVENT_AUTORUN,  
    SPF_EVENT_MODULE_QUIT, 
    SPF_EVENT_FIN1, 
    SPF_EVENT_FIN2  
};



#define SPF_MOD_IOCTL_GET_MOD_INFO 1000001


typedef struct {
    U32 ver;
    char compile_time[32];
}SPF_MOD_INFO_S;

typedef struct {
    U32 event;
    U32 stop: 1; 
}SPF_EVOB_CTX_S;

#ifdef __cplusplus
}
#endif
#endif 
