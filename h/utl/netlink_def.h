/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _NETLINK_DEF_H_
#define _NETLINK_DEF_H_

#include <linux/netlink.h>
#include <linux/genetlink.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NETLINK_GEN_NAME "my_nl_gen"

enum{  
    NETLINK_GEN_C_UNSPEC,  
    NETLINK_GEN_C_CMD,

    NETLINK_GEN_C_MAX
};  

typedef struct netlink_msg_st {
    struct nlmsghdr hdr;
    struct genlmsghdr g;  
    int msg_type;
    int data_len;
    int reply_size; 
    void *reply_ptr;
    char data[0];
}NETLINK_MSG_S;

#ifdef __cplusplus
}
#endif
#endif 
