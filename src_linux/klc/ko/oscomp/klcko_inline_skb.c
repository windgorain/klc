/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/stddef.h>
#include <linux/kmod.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/list.h>
#include <linux/version.h>
#include <linux/bvec.h>
#include <linux/dma-mapping.h>
#include <linux/siphash.h>
#include <linux/bug.h>
#include <linux/netdev_features.h>
#include <net/checksum.h>
#include <net/flow.h>
#include <net/flow_dissector.h>

#include "klcko_inline.h"

int __skb_unclone_keeptruesize(struct sk_buff *skb, gfp_t pri) { return -1; }
#undef _LINUX_SKBUFF_H
#include <linux/skbuff.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

int KlcKoInline_InitSkb(void)
{
    return 0;
}

