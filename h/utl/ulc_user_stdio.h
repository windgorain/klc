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

#ifdef IN_ULC_USER

typedef unsigned long size_t;
typedef struct FILE  FILE;

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

#endif

#ifdef __cplusplus
}
#endif
#endif 
