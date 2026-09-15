/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _IP_BPF_H
#define _IP_BPF_H
#include "utl/ip4_utl.h"
#include "bpf/eth_bpf.h"
#ifdef __cplusplus
extern "C"
{
#endif

static inline void * IPBPF_GetIPHeader(void *ip_hdr, void *data_end)
{
    IP_HEAD_S *iph = ip_hdr;

    if ((char*)(iph + 1) > (char*)data_end) {
        return NULL;
    }

    return iph;
}

static inline void * IPBPF_GetIPHeaderFromEth(void *data, void *data_end)
{
    ETH_HEADER_S * eth = ETHBPF_GetEthHeader(data, data_end);
    if (! eth) {
        return NULL;
    }

    if (eth->usProto != htons(ETH_P_IP)) {
        return NULL;
    }

    return IPBPF_GetIPHeader(eth+1, data_end);
}


static inline void _IPBPF_BuildHeader(OUT IP_HEAD_S *iphdr, U8 proto, U32 sip, U32 dip, U16 total_len)
{
    iphdr->ucVer = 0x4;
    iphdr->ucHLen = 0x5;
    iphdr->usOff = 0;

    iphdr->ucTos = 0;
    iphdr->ucProto = proto;

    iphdr->unSrcIp.uiIp = sip;
    iphdr->unDstIp.uiIp = dip;
    iphdr->ucTtl = 255;

    iphdr->usTotlelen = htons(total_len);

    iphdr->usCrc = 0;
}

#define IPBPF_BuildHeader(iphdr, proto, sip, dip, total_len, pkt_id) do { \
    _IPBPF_BuildHeader(iphdr, proto, sip, dip, total_len); \
    (iphdr)->usIdentification = pkt_id; \
}while(0)

static inline void IPBPF_SwapIP(IP_HEAD_S *ip_hdr)
{
    U32 tmp = ip_hdr->unDstIp.uiIp;
    ip_hdr->unDstIp.uiIp = ip_hdr->unSrcIp.uiIp;
    ip_hdr->unSrcIp.uiIp = tmp;
}


static inline void * IPBPF_Payload(IP_HEAD_S *ip_hdr)
{
    return (void*)ip_hdr + IP_HEAD_LEN(ip_hdr);
}


static inline U16 IPBPF_PayloadLen(IP_HEAD_S *ip_hdr)
{
    U16 ip_len = ntohs(ip_hdr->usTotlelen);
    return ip_len - IP_HEAD_LEN(ip_hdr);
}

#ifdef __cplusplus
}
#endif
#endif 
