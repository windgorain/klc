/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-29
* Description: 
******************************************************************************/
#ifndef _DNS_DEF_H_
#define _DNS_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DNS_MAX_DOMAIN_NAME_LEN  253    
#define DNS_CDN_DOMAIN_NAME_LEN  63     
#define DNS_MAX_DNS_NAME_SIZE   (DNS_MAX_DOMAIN_NAME_LEN + 2) 
#define DNS_MAX_LABEL_LEN     63        
#define DNS_MAX_LABEL_NUM     ((DNS_MAX_DOMAIN_NAME_LEN + 1) / 2)
#define DNS_MAX_UDP_PKT_SIZE  512

#define DNS_IP_MAX            100 
#define DNS_DFT_SERVER_PORT   53 


#define DNS_TYPE_NULL   0
#define DNS_TYPE_A      1
#define DNS_TYPE_NS     2
#define DNS_TYPE_CNAME  5
#define DNS_TYPE_SOA    6
#define DNS_TYPE_PTR    12
#define DNS_TYPE_HINFO  13
#define DNS_TYPE_MX     15
#define DNS_TYPE_TXT    16
#define DNS_TYPE_AAAA   28
#define DNS_TYPE_AXFR   252
#define DNS_TYPE_ANY    255


#define DNS_FLAG_RESPONES           1
#define DNS_FLAG_OPCODE_STANDARY    0
#define DNS_FLAG_REPLY_CODE         0



#define DNS_CLASS_IN 1

#define DNS_RRTYPE_QUERY 0
#define DNS_RRTYPE_ANSWER 1
#define DNS_RRTYPE_AUTHORITY 2
#define DNS_RRTYPE_ADDITIONAL 3


#ifdef __cplusplus
}
#endif
#endif 
