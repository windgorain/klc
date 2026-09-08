/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      lixingang
* Description: 
* History:     
******************************************************************************/
#include "utl/int_types.h"
#include "utl/ulc_user.h"
#include "utl/mybpf_spf_sec.h"
#include "spf/spf_def.h"

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

