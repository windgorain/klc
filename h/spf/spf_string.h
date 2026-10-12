/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-12
* Description: 
******************************************************************************/
#ifndef _SPF_STRING_H_
#define _SPF_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#define memset _memset
#define memcmp _memcmp
#define memcpy _memcpy
#define strlen _strlen
#define strnstr _strnstr
#define strcmp _strcmp
#define atol _atol
#define atoi _atoi

static inline void * memset(void *s, int c, size_t count)
{
	char *xs = s;
	while (count--)
		*xs++ = c;
	return s;
}

static inline int memcmp(const void *cs, const void *ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

static inline void * memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}

static inline size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		;
	return sc - s;
}

static inline char *strnstr(const char *s1, const char *s2, size_t len)
{
	size_t l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	while (len >= l2) {
		len--;
		if (!_memcmp(s1, s2, l2))
			return (char *)s1;
		s1++;
	}
	return NULL;
}

static inline int strcmp(const char *cs, const char *ct)
{
	unsigned char c1, c2;
	while (1) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}

static inline long atol(const char *s)
{
	unsigned long ret = 0;
	unsigned long d;
	int neg = 0;

	if (*s == '-') {
		neg = 1;
		s++;
	}

	while (1) {
		d = (*s++) - '0';
		if (d > 9)
			break;
		ret *= 10;
		ret += d;
	}

	return neg ? -ret : ret;
}

static inline int atoi(const char *s)
{
	return atol(s);
}
#ifdef __cplusplus
}
#endif
#endif 
