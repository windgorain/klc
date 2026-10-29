/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-29
* Description: 
******************************************************************************/
#ifndef _DNS_FUNC_H_
#define _DNS_FUNC_H_

#include "dns_def.h"

#ifdef __cplusplus
extern "C" {
#endif


static inline int DNS_DomainName2DnsName(char *domain_name, OUT char *dns_name, unsigned int dns_name_size)
{
    char *ptr = domain_name;
    unsigned int lable_len = 0;
    unsigned int off = 0;
    char tmp;

    dns_name[0] = '\0';

    if (strlen(domain_name) + 1 >= dns_name_size) {
        return -1;
    }

    while (1) {
        tmp = *ptr;
        if ((tmp != '.') && (tmp != '\0')) {
            lable_len ++;
            dns_name[off + lable_len] = tmp;
        } else {
            if (lable_len > DNS_MAX_LABEL_LEN) {
                dns_name[0] = '\0';
                return -1;
            }

            dns_name[off] = lable_len;
            if (lable_len > 0) {
                off += (lable_len + 1);
                lable_len = 0;
            }
        }

        if (tmp == '\0') {
            break;
        }

        ptr++;
    }

    dns_name[off] = '\0';

    return off;
}


static inline int DNS_DnsName2DomainName(char *dns_name, OUT char *domain_name, unsigned int domain_name_size)
{
    char *ptr = dns_name;
    char *out = domain_name;

    domain_name[0] = '\0';

    if (strlen(dns_name) > domain_name_size) { 
        return -1;
    }

    while (1) {
        unsigned char len = (unsigned char)(*ptr);
        ptr ++;

        if (len == 0) {
            break;
        }

        if (out != domain_name) {
            *out = '.';
            out ++;
        }

        for (int i=0; i<len; i++) {
            if (*ptr == '\0') { 
                return -1;
            }
            *out++ = *ptr++;
        }
    }

    *out = '\0';

    return out - domain_name;
}

#ifdef __cplusplus
}
#endif
#endif 
