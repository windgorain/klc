/*********************************************************
*   Copyright (C) LiXingang
*   Description: 给ULC用户使用的头文件
*
********************************************************/
#ifndef _ULC_USER_H
#define _ULC_USER_H

#ifdef IN_ULC_USER

#include <stdio.h>
#include <stddef.h>
#include "utl/int_types.h"
#include "utl/types.h"
#include "utl/types_utl.h"
#include "utl/bpf_helper_utl.h"
#include "utl/int_types.h"
#include "utl/ulc_def.h"
#include "utl/ulc_user_def.h"
#include "utl/ulc_user_base.h"
#include "utl/ulc_user_sys.h"
#include "utl/ulc_user_user.h"
#include "utl/mybpf_spf_sec.h"
#include "utl/mybpf_map_def.h"

#ifndef htons
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ntohl(x) __builtin_bswap32(x)
#define ntohs(x) __builtin_bswap16(x)
#define htonl(x) __builtin_bswap32(x)
#define htons(x) __builtin_bswap16(x)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define ntohl(x) (x)
#define ntohs(x) (x)
#define htonl(x) (x)
#define htons(x) (x)
#else
#error "error"
#endif
#endif

#ifndef noinline
#define noinline __attribute__((noinline))
#endif

#undef SEC
#define SEC(NAME) __attribute__((section(NAME), used))

#ifndef NULL
#define NULL 0
#endif


#endif

#endif 
