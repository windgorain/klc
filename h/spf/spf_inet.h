/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-30
* Description: 
******************************************************************************/
#ifndef _SPF_INET_H_
#define _SPF_INET_H_

#include "utl/ctype_utl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define inet_aton _spf_inet_aton
#define inet_addr _spf_inet_addr
#define inet_ntop _spf_inet_ntop


static inline int inet_aton(const char *cp, struct in_addr *addrptr)
{
	U32 addr;
	int value;
	int part;

	if (cp == NULL) {
		return 0;
	}

	addr = 0;
	for (part = 1; part <= 4; part++) {

		if (!ISDIGIT(*cp))
			return 0;

		value = 0;
		while (ISDIGIT(*cp)) {
			value *= 10;
			value += *cp++ - '0';
			if (value > 255)
				return 0;
		}

		if (part < 4) {
			if (*cp++ != '.')
				return 0;
		} else {
			char c = *cp++;
			if (c != '\0' && !ISSPACE(c))
				return 0;
		}

		addr <<= 8;
		addr |= value;
	}

	if (addrptr) {
		addrptr->s_addr = htonl(addr);
	}

	return 1;
}


static inline U32 inet_addr(const char *cp)
{
	struct in_addr a;

	if (!inet_aton(cp, &a))
		return INADDR_NONE;
	else
		return a.s_addr;
}


static inline const char *inet_ntop(int af, const void *restrict a0, char *restrict s, int l)
{
	const unsigned char *a = a0;
	int i, j, max, best;
	char buf[100];

	switch (af) {
	case AF_INET:
		if (snprintf(s, l, "%d.%d.%d.%d", a[0],a[1],a[2],a[3]) < l)
			return s;
		break;
	case AF_INET6:
		if (memcmp(a, "\0\0\0\0\0\0\0\0\0\0\377\377", 12))
			snprintf(buf, sizeof buf,
				"%x:%x:%x:%x:%x:%x:%x:%x",
				256*a[0]+a[1],256*a[2]+a[3],
				256*a[4]+a[5],256*a[6]+a[7],
				256*a[8]+a[9],256*a[10]+a[11],
				256*a[12]+a[13],256*a[14]+a[15]);
		else
			snprintf(buf, sizeof buf,
				"%x:%x:%x:%x:%x:%x:%d.%d.%d.%d",
				256*a[0]+a[1],256*a[2]+a[3],
				256*a[4]+a[5],256*a[6]+a[7],
				256*a[8]+a[9],256*a[10]+a[11],
				a[12],a[13],a[14],a[15]);
		
		for (i=best=0, max=2; buf[i]; i++) {
			if (i && buf[i] != ':') continue;
			j = strspn(buf+i, ":0");
			
			if (j>max+(best==0)) best=i, max=j;
		}
		if (max>3) {
			buf[best] = buf[best+1] = ':';
			memmove(buf+best+2, buf+best+max, i-best-max+1);
		}
		if (strlen(buf) < l) {
			strcpy(s, buf);
			return s;
		}
		break;
	default:
		return 0;
	}
	return 0;
}


#ifdef __cplusplus
}
#endif
#endif 
