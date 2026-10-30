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


#define DNS_HEAD_LEN 12
#define DNS_QST_LEN 4   
#define DNS_RR_NAME_LEN 2 

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


#pragma pack(1)

typedef struct tagDNS_Header
{
    U16    usTransID;    
    
#if BS_BIG_ENDIAN
    
    U16    usQR: 1;          
    U16    usOpcode: 4;      
    U16    usAA: 1;          
    U16    usTC: 1;          
    U16    usRD: 1;          

    
    U16    usRA: 1;          
    U16    usUnused :1;      
    U16    usAD: 1;          
    U16    usCD: 1;          
    U16    usRcode :4;       
#elif BS_LITTLE_ENDIAN
    
    U16    usRD :1;          
    U16    usTC :1;          
    U16    usAA :1;          
    U16    usOpcode :4;      
    U16    usQR :1;          

    
    U16    usRcode :4;       
    U16    usCD: 1;          
    U16    usAD: 1;          
    U16    usUnused :1;      
    U16    usRA :1;          
#else
    #error "Error"
#endif
    
    U16    usQdCount;        
    U16    usAnCount;        
    U16    usNsCount;        
    U16    usArCount;        
}DNS_HEADER_S;


typedef struct tagDNS_Question {
    U16  usType;                       
    U16  usClass;                      
}DNS_QST_S; 


typedef struct {
    U16  usType;
    U16  usClass;
    U32  uiTTL;
    U16  usRDLen;
} DNS_RR_S;

#pragma pack()

#ifdef __cplusplus
}
#endif
#endif 
