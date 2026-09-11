/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _ETH_BPF_H
#define _ETH_BPF_H

#include "utl/eth_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

static inline ETH_HEADER_S * ETHBPF_GetEthHeader(void *data, void *data_end)
{
	ETH_HEADER_S *eth = data;

    if ((char*)(eth + 1) > (char*)data_end) {
        return NULL;
    }

    return eth;
}

static inline void ETHBPF_SwapMac(ETH_HEADER_S *eth_hdr)
{
    MAC_ADDR_S tmp = eth_hdr->stDMac;
    eth_hdr->stDMac = eth_hdr->stSMac;
    eth_hdr->stSMac = tmp;
}

#ifdef __cplusplus
}
#endif
#endif 
