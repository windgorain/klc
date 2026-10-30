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

#define memset _spf_memset
#define memcmp _spf_memcmp
#define memcpy _spf_memcpy
#define memchr _spf_memchr
#define memmove _spf_memmove
#define strlen _spf_strlen
#define strnlen _spf_strnlen
#define strnstr _spf_strnstr
#define strstr _spf_strstr
#define strcmp _spf_strcmp
#define strncmp _spf_strncmp
#define strcpy _spf_strcpy
#define strlcpy _spf_strlcpy
#define strscpy _spf_strscpy
#define strspn _spf_strspn
#define atol _spf_atol
#define atoi _spf_atoi

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

void * memchr(const void *s, int c, size_t n)
{
    const unsigned char *p = s;
    while (n-- != 0) {
        if ((unsigned char)c == *p++) {
            return (void *)(p - 1);
        }
    }
    return NULL;
}

static inline void * memmove(void *dest, const void *src, size_t count)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		tmp = dest;
		s = src;
		while (count--)
			*tmp++ = *s++;
	} else {
		tmp = dest;
		tmp += count;
		s = src;
		s += count;
		while (count--)
			*--tmp = *--s;
	}
	return dest;
}

static inline size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		;
	return sc - s;
}

static inline size_t strnlen(const char *a, size_t max_len)
{
    const char *end = memchr(a, '\0', max_len);
    return end ? end - (char*)a : max_len;
}

static inline char *strnstr(const char *s1, const char *s2, size_t len)
{
	size_t l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	while (len >= l2) {
		len--;
		if (!_spf_memcmp(s1, s2, l2))
			return (char *)s1;
		s1++;
	}
	return NULL;
}


static inline char * strstr(const char *s1, const char *s2)
{
	size_t l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1, s2, l2))
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

static inline int strncmp(const char *cs, const char *ct, size_t count)
{
	unsigned char c1, c2;

	while (count) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

static inline size_t strspn(const char *s, const char *accept)
{
    const char *p;
    const char *a;
    size_t count = 0;

    for (p = s; *p != '\0'; ++p) {
        for (a = accept; *a != '\0'; ++a)
            if (*p == *a)
                break;
        if (*a == '\0')
            return count;
        else
            ++count;
    }

    return count;
}

static inline char *strcpy(char *dst, const char *src)
{
	char *ret = dst;
	while ((*dst++ = *src++));
	return ret;
}

static inline size_t strlcpy(char *dst, const char *src, size_t size)
{
    unsigned long n;
    char *p;

    for (p = dst, n = 0; n + 1 < size && *src!= '\0';  ++p, ++src, ++n) {
        *p = *src;
    }
    *p = '\0';
    if(*src == '\0') {
        return n;
    } else {
        return n + strlen(src);
    }
}

static inline long strscpy(char *dst, const char *src, size_t count)
{
	size_t len;

	if (count == 0)
		return -1;
	len = strnlen(src, count - 1);
	memcpy(dst, src, len);
	dst[len] = '\0';
	return src[len] ? -1: len;
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
