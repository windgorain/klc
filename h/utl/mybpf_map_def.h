/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#ifndef _MYBPF_MAP_DEF_H_
#define _MYBPF_MAP_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MYBPF_BPF_PINNING_PIN       1
#define MYBPF_BPF_PINNING_GLOBAL_NS 2
#define MYBPF_BPF_PINNING_NAMEMAP   0x01000000 

typedef struct {
	unsigned int type;
	unsigned int key_size;
	unsigned int value_size;
	unsigned int max_entries;
	unsigned int map_flags;
	unsigned int id;
	unsigned int pinning;
}MYBPF_MAP_DEF_S;

static inline int MYBPF_MAP_IsPinning(unsigned int pinning)
{
    if ((pinning == MYBPF_BPF_PINNING_PIN) || (pinning == MYBPF_BPF_PINNING_GLOBAL_NS)) {
        return 1;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
#endif 
