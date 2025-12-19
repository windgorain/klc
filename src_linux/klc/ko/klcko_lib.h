/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _KLCKO_LIB_H
#define _KLCKO_LIB_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/errno.h>
#include <linux/stddef.h>
#include <linux/socket.h>
#include <linux/kmod.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/ip.h>
#include <linux/netfilter_ipv4.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/list.h>
#include <linux/version.h>
#include <linux/bpf.h>
#include <net/cls_cgroup.h>
#include <net/genetlink.h>
#include "utl/netlink_def.h"

#include "klc/klc_def.h"
#include "klc/klctool_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef u64  (*PF_KLC_HELPER)(u64 p1, u64 p2, u64 p3, u64 p4, u64 p5);

int KlcKo_Init(void);
void KlcKo_Fini(void);

int KlcKoInline_Init(void);

int KlcKoNl_Init(void);
void KlcKoNl_Fini(void);
typedef int (*PF_KLCKO_NL_MSG)(int cmd, void *msg, int data_len, void *reply, int reply_size, OUT int *reply_len);
int KlcKoNl_Reg(unsigned int type, PF_KLCKO_NL_MSG func);

int KlcKoFunc_Init(void);

int KlcKoConfig_Init(void);
void KlcKoConfig_Fini(void);

#define KLCKO_BASE_FUNC_NUM 256
int KlcKo_SetBaseFunc(U32 index, void *func);
void KlcKo_BaseEnable(int enable);
void klc_base_module_use(int inc);

#ifdef __cplusplus
}
#endif
#endif 
