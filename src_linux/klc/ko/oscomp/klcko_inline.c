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
#include <linux/cgroup.h>
#include <linux/delay.h>
#include <linux/memcontrol.h>
#include <linux/prefetch.h>
#include <linux/in.h>
#include <linux/string.h>
#include <linux/hashtable.h>
#include <uapi/linux/netdevice.h>
#include <net/netprio_cgroup.h>
#include <linux/dynamic_queue_limits.h>

#include "klcko_inline.h"
#include <linux/crypto.h>
#include <crypto/skcipher.h>
#include <net/net_namespace.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

int KlcKoInline_Init(void)
{
    return 0;
}

