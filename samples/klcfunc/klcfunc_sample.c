/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang
* Description: 展示如何使用klc function支持循环, 以及如何查找字符串
* History:     
******************************************************************************/
#include "utl/int_types.h"
#include "utl/ulc_user.h"
#include "utl/mybpf_spf_sec.h"
#include "spf/spf_def.h"
#include "spf/spf_string.h"

#define NAMEFUNC_TEST "namefunc_test"
#define TEST_ID 10001022


SEC_NAME_FUNC(NAMEFUNC_TEST)
U64 _test1(void *data, void *data_end)
{
    int i;
    int len = data_end - data;
    U8 *d = data;
    U64 sum = 0;

    for (i=0; i<len; i++) {
        sum += d[i];
    }

    return sum;
}


SEC_ID_FUNC(TEST_ID)
U64 _test2(void *data, void *data_end)
{
    int i;
    int len = data_end - data;
    U8 *d = data;
    U64 sum = 0;

    for (i=0; i<len; i++) {
        sum += d[i];
    }

    return sum;
}

SEC_NAME_FUNC("find_host")
int _find_host(void *data, void *data_end, OUT char *host, int host_size)
{
    void * host_begin = strnstr(data, "\r\nHost:", data_end - data);
    if (! host_begin) {
        return -1;
    }

    host_begin += 2;

    void * host_end = strnstr(host_begin, "\r\n", data_end - host_begin);
    if (! host_end) {
        return -1;
    }

    int host_len = host_end - host_begin;
    int copy_len = MIN(host_size - 1, host_len);
    memcpy(host, host_begin, copy_len);
    host[copy_len] = '\0';

    return 0;
}

