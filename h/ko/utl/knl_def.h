/*********************************************************
*   Copyright (C) LiXingang
*   Description: ko netlink
*
********************************************************/
#ifndef _KNL_DEF_H
#define _KNL_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    IN int ele_size;  
    IN int max_count; 
    OUT int count; 
    IN int from;  
    OUT int pos;   
}KNL_GET_S;

typedef struct {
    IN int dump_size;
    OUT int write_size;
    unsigned char data[0];  
}KNL_DUMP_S;

typedef int (*PF_KUTL_NL_FILL)(void *node, void *dst, int user_size, void *ud);

#ifdef __cplusplus
}
#endif
#endif 
