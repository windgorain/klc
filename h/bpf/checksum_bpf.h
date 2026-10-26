/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _CHECKSUM_BPF_H_
#define _CHECKSUM_BPF_H_

#ifdef __cplusplus
extern "C" {
#endif


static inline unsigned short CHKSUMBPF_CheckSum(void *data, int bufsz )
{
    volatile unsigned short *buf = data;
    unsigned int sum = 0;

    while (bufsz > 1) {
        sum += *buf;
        buf++;
        bufsz -= 2;
    }

    if (bufsz == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

static inline unsigned short CHKSUMBPF_TcpCheckSum(void *tcphdr, int tcp_len, U32 sip, U32 dip)
{
    volatile unsigned short *buf;
    unsigned int sum = 0;
    int len = tcp_len;

    buf = tcphdr;
    while (len > 1) {
        sum += *buf;
        buf++;
        len -= 2;
    }

    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    buf = (void*)&sip;
    sum += buf[0];
    sum += buf[1];

    buf = (void*)&dip;
    sum += buf[0];
    sum += buf[1];

    sum += htons(IP_PROTO_TCP);
    sum += htons(tcp_len);

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}




static inline unsigned short CHKSUMBPF_Change(volatile U16 *olddata, int oldcount, volatile U16 *newdata, int newcount, U16 old_checksum)
{
    unsigned int sum;
    int i;

    sum = (~old_checksum) & 0xffff;

    for (i=0; i<oldcount; i++) {
        sum += (~olddata[i] & 0xffff);
    }
    for (i=0; i<newcount; i++) {
        sum += newdata[i];
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

#ifdef __cplusplus
}
#endif
#endif 
