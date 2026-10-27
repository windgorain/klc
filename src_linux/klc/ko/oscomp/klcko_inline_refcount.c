/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include <linux/kernel.h>
#include <linux/bug.h>
#include <linux/preempt.h>

#include "klcko_inline.h"

#undef _LINUX_REFCOUNT_H
#include <linux/refcount.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

int KlcKoInline_InitRefcount(void)
{
    return 0;
}

