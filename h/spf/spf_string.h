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

#define memcmp _memcmp
#define memcpy _memcpy
#define strlen _strlen
#define strnstr _strnstr

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

#ifdef __cplusplus
}
#endif
#endif 
