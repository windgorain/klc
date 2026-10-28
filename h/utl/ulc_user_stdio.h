/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-26
* Description: 
******************************************************************************/
#ifndef _ULC_USER_STDIO_H_
#define _ULC_USER_STDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_ULC_STDIO

#ifndef PF_INET
    #define PF_INET         2
    #define PF_INET6        10

    #define AF_INET         PF_INET
    #define AF_INET6        PF_INET6

    #define SOCK_STREAM    1
    #define SOCK_DGRAM     2

    #define	INADDR_NONE		((unsigned long int) 0xffffffff)

    typedef unsigned short int sa_family_t;
    typedef unsigned int in_addr_t;
    struct in_addr { in_addr_t s_addr; };
    
    struct sockaddr {
    	sa_family_t sa_family;
    	char sa_data[14];
    };
    
    struct sockaddr_in {
    	sa_family_t sin_family;
    	unsigned short sin_port;
    	struct in_addr sin_addr;
    	unsigned char sin_zero[8];
    };

#endif

#ifndef SHUT_RD
#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
#endif

#ifndef F_GETFL
#define O_CREAT        0100
#define O_EXCL         0200
#define O_NOCTTY       0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_DSYNC      010000
#define O_SYNC     04010000
#define O_RSYNC    04010000
#define O_DIRECTORY  040000
#define O_NOFOLLOW  0100000
#define O_CLOEXEC  02000000

#define O_ASYNC      020000
#define O_DIRECT    0200000
#define O_LARGEFILE 0400000
#define O_NOATIME  01000000
#define O_PATH    010000000
#define O_TMPFILE 020040000
#define O_NDELAY O_NONBLOCK

#define F_DUPFD  0
#define F_GETFD  1
#define F_SETFD  2
#define F_GETFL  3
#define F_SETFL  4

#define F_SETOWN 8
#define F_GETOWN 9
#define F_SETSIG 10
#define F_GETSIG 11

#define F_GETLK 12
#define F_SETLK 13
#define F_SETLKW 14

#define F_SETOWN_EX 15
#define F_GETOWN_EX 16

#define F_GETOWNER_UIDS 17
#endif

#ifndef NF_DROP 
#define NF_DROP 0
#define NF_ACCEPT 1
#define NF_STOLEN 2
#define NF_QUEUE 3
#define NF_REPEAT 4
#define NF_STOP 5	
#define NF_MAX_VERDICT NF_STOP

enum {
	NFPROTO_UNSPEC =  0,
	NFPROTO_INET   =  1,
	NFPROTO_IPV4   =  2,
	NFPROTO_ARP    =  3,
	NFPROTO_NETDEV =  5,
	NFPROTO_BRIDGE =  7,
	NFPROTO_IPV6   = 10,
	NFPROTO_DECNET = 12,
	NFPROTO_NUMPROTO,
};

enum nf_inet_hooks {
	NF_INET_PRE_ROUTING,
	NF_INET_LOCAL_IN,
	NF_INET_FORWARD,
	NF_INET_LOCAL_OUT,
	NF_INET_POST_ROUTING,
	NF_INET_NUMHOOKS,
	NF_INET_INGRESS = NF_INET_NUMHOOKS,
};
#endif

typedef unsigned long size_t;
typedef struct FILE  FILE;
typedef long jmp_buf[32];

extern int printf(const char *format, ...);
extern int sprintf(char *str, const char *format, ...);
extern unsigned long int strtoul(const char *nptr, char **endptr, int base);
extern void *malloc(size_t size);
extern void free(void *ptr);
extern void *realloc(void *ptr, size_t size);
extern void *calloc(size_t nmemb, size_t size);
extern int atoi(const char *nptr);
extern unsigned long long strtoull(const char *nptr, char **endptr, int base);
extern void abort(void);
extern unsigned int inet_addr(const char *cp);
extern void *memset(void *s, int c, size_t count);

#endif

#ifdef __cplusplus
}
#endif
#endif 
