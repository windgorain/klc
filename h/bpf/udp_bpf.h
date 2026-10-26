/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _UDP_BPF_H
#define _UDP_BPF_H

#include "utl/ip4_utl.h"
#include "utl/udp_utl.h"
#include "ip_bpf.h"

#ifdef __cplusplus
extern "C"
{
#endif

static inline void * UDPBPF_GetUdpHeader(void *udp_hdr, void *data_end)
{
    UDP_HEAD_S *udph = udp_hdr;

    if ((char*)(udph + 1) > (char*)data_end) {
        return NULL;
    }

    return udph;
}

static inline void * UDPBPF_GetUdpHeaderFromIP(void *piph, void *data_end)
{
    IP_HEAD_S *iph = piph;

    if (iph->ucProto != IP_PROTO_UDP) {
        return NULL;
    }

    int iph_len = iph->ucHLen << 2;
    void *pudph = (char*)iph + iph_len;

    return UDPBPF_GetUdpHeader(pudph, data_end);
}

static inline void * UDPBPF_GetUdpHeaderFromEth(void *eth_hdr, void *data_end)
{
    IP_HEAD_S *iph = IPBPF_GetIPHeaderFromEth(eth_hdr, data_end);
    if (! iph) {
        return NULL;
    }

    return UDPBPF_GetUdpHeaderFromIP(iph, data_end);
}

#ifdef __cplusplus
}
#endif
#endif 
