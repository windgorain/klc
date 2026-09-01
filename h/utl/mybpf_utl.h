/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _MYBPF_UTL_H
#define _MYBPF_UTL_H

#include "mybpf_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    int sec_id;        
    int map_def_size;
    int map_count;
    void *maps;
}MYBPF_MAPS_SEC_S;

typedef struct {
	UCHAR opcode;		
	UCHAR dst_reg:4;	
	UCHAR src_reg:4;	
	short off;	    	
	int imm;	    	
}MYBPF_INSN_S;

#ifdef __cplusplus
}
#endif
#endif 
