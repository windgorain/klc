/*================================================================
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2016-10-2
* Description: 
* History:     
================================================================*/
#ifndef _KO_UTL_H
#define _KO_UTL_H

#include "ko_errcode.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define KO_Print(fmt, ...) \
    do { \
        printk_ratelimited(KERN_INFO fmt, ##__VA_ARGS__); \
    } while(0)

#define KO_PrintLn() \
    do { \
        printk_ratelimited(KERN_INFO "Line: %d\n", __LINE__); \
    } while(0)

#ifndef list_next_or_null_rcu
#define list_next_or_null_rcu(head, ptr, type, member) \
({ \
	struct list_head *__head = (head); \
	struct list_head *__ptr = (ptr); \
	struct list_head *__next = READ_ONCE(__ptr->next); \
	likely(__next != __head) ? list_entry_rcu(__next, type, \
						  member) : NULL; \
})
#endif

#ifdef setup_timer
#define KO_SETUP_TIMER setup_timer
#else
#define KO_SETUP_TIMER timer_setup
#endif

#ifdef __cplusplus
}
#endif
#endif 
