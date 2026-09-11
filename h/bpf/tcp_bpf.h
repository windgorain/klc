/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TCP_BPF_H
#define _TCP_BPF_H

#include "utl/tcp_def.h"
#include "bpf/ip_bpf.h"

#ifdef __cplusplus
extern "C"
{
#endif

static inline void * TCPBPF_GetTcpHeader(void *tcp_hdr, void *data_end)
{
    TCP_HEAD_S *tcph = tcp_hdr;

    if ((char*)(tcph + 1) > (char*)data_end) {
        return NULL;
    }

    return tcph;
}

static inline void * TCPBPF_GetTcpHeaderFromIP(void *piph, void *data_end)
{
    IP_HEAD_S *iph = piph;

    if (! iph) {
        return NULL;
    }

    if (iph->ucProto != IP_PROTO_TCP) {
        return NULL;
    }

    int iph_len = iph->ucHLen << 2;
    void *ptcph = (char*)iph + iph_len;

    return TCPBPF_GetTcpHeader(ptcph, data_end);
}

static inline void * TCPBPF_GetTcpHeaderFromEth(void *data, void *data_end)
{
    IP_HEAD_S *iph = IPBPF_GetIPHeaderFromEth(data, data_end);
    if (! iph) {
        return NULL;
    }

    return TCPBPF_GetTcpHeaderFromIP(iph, data_end);
}


static inline BOOL_T TCPBPF_IsSynPkt(void *data, void *data_end)
{
    IP_HEAD_S *iph;

    iph = IPBPF_GetIPHeaderFromEth(data, data_end);
    if (! iph) {
        return FALSE;
    }

    if (iph->ucProto != IP_PROTO_TCP) {
        return FALSE;
    }

    TCP_HEAD_S *tcp_hdr = TCPBPF_GetTcpHeaderFromIP(iph, data_end);
    if (! tcp_hdr) {
        return FALSE;
    }

    if (TCP_FLAG_IS_SYN(tcp_hdr->ucFlag)) {
        return TRUE;
    }

    return FALSE;
}

static inline void TCPBPF_SwapPort(TCP_HEAD_S *tcp_hdr)
{
    U16 tmp = tcp_hdr->usDstPort;
    tcp_hdr->usDstPort = tcp_hdr->usSrcPort;
    tcp_hdr->usSrcPort = tmp;
}

#ifdef __cplusplus
}
#endif
#endif 
