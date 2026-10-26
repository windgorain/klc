/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-11
* Description: 
******************************************************************************/
#ifndef _IPCSUM_BPF_H_
#define _IPCSUM_BPF_H_

#include "bpf/checksum_bpf.h"

#ifdef __cplusplus
extern "C" {
#endif


static inline void IPBPF_ChangeIP(IP_HEAD_S *ip_hdr, U32 new_sip, U32 new_dip)
{
    U32 new_ip[2];

    new_ip[0] = new_sip;
    new_ip[1] = new_dip;

    ip_hdr->usCrc =  CHKSUMBPF_Change((void*)&ip_hdr->unSrcIp, 4, (void*)new_ip, 4, ip_hdr->usCrc);

    ip_hdr->unSrcIp.uiIp = new_sip;
    ip_hdr->unDstIp.uiIp = new_dip;
}


static inline void IPBPF_Checksum(IP_HEAD_S *iphdr)
{
    iphdr->usCrc = 0;
    iphdr->usCrc = CHKSUMBPF_CheckSum(iphdr, sizeof(IP_HEAD_S));
}

#ifdef __cplusplus
}
#endif
#endif 
