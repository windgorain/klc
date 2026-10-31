/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-30
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_NOTIFIER_H_
#define _KLC_SYM_NOTIFIER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NOTIFY_DONE		    0x0000		
#define NOTIFY_OK		    0x0001		
#define NOTIFY_STOP_MASK	0x8000		
#define NOTIFY_BAD	        (NOTIFY_STOP_MASK|0x0002) 
#define NOTIFY_STOP	 	    (NOTIFY_OK|NOTIFY_STOP_MASK)


#define KBD_KEYCODE		    0x0001 
#define KBD_UNBOUND_KEYCODE	0x0002 
#define KBD_UNICODE		    0x0003 
#define KBD_KEYSYM		    0x0004 
#define KBD_POST_KEYSYM		0x0005 

#define KEY_F12			    88

struct keyboard_notifier_param {
	struct vc_data *vc;	
	int down;		
	int shift;		
	int ledstate;		
	unsigned int value;	
};

struct keyboard_notifier_param32 {
	unsigned int vc;	
	int down;		
	int shift;		
	int ledstate;		
	unsigned int value;	
};

typedef	int (*notifier_fn_t)(void *nb, unsigned long action, void *data);

struct notifier_block64 {
	notifier_fn_t notifier_call;
	struct notifier_block *next;
	int priority; 
};

struct notifier_block32 {
	unsigned int notifier_call;
	unsigned int next;
	int priority; 
    unsigned int raw_call; 
};

typedef union {
    struct notifier_block64 nb64;
    struct notifier_block32 nb32;
}KLC_NOTIFIER_BLOCK_S;

static inline int _klc_notifier_cb32(U64 p1, U64 p2)
{
    KLC_NOTIFIER_BLOCK_S *nb = (void*)(long)(p1 & 0xffffffff);
    unsigned long action = (p1 >> 32);
    void *data = (void*)(long)(p2 & 0xffffffff);

    notifier_fn_t raw_fn = (void*)(long)(nb->nb32.raw_call);

    return raw_fn(nb, action, data);
}

static inline int _klc_keyboard_notifier_cb32(U64 p1, U64 p2)
{
    KLC_NOTIFIER_BLOCK_S *nb = (void*)(long)(p1 & 0xffffffff);
    unsigned long action = (p1 >> 32);
    struct keyboard_notifier_param32 *d32 = (void*)(long)(p2 & 0xffffffff);

    notifier_fn_t raw_fn = (void*)(long)(nb->nb32.raw_call);

    struct keyboard_notifier_param d;

    d.vc = (void*)(long)d32->vc;
    d.down = d32->down;
    d.shift = d32->shift;
    d.ledstate = d32->ledstate;
    d.value = d32->value;

    return raw_fn(nb, action, &d);
}

static inline void init_register_notifier(void *fn, int pri, OUT KLC_NOTIFIER_BLOCK_S *nb)
{
    if (klc_is_sys32()) {
        nb->nb32.notifier_call = (long)(void*)_klc_notifier_cb32;
        nb->nb32.priority = pri;
        nb->nb32.raw_call = (int)(long)fn;
    } else {
        nb->nb64.notifier_call = fn;
        nb->nb64.priority = pri;
    }
}


static inline int register_keyboard_notifier(KLC_NOTIFIER_BLOCK_S *nb)
{
    static int (*register_keyboard_notifier_fn)(void *) = NULL;
    if (! register_keyboard_notifier_fn) {
        register_keyboard_notifier_fn = ulc_sys_get_sym("register_keyboard_notifier");
        if (! register_keyboard_notifier_fn) {
            return -1;
        }
    }

    if (klc_is_sys32()) {
        nb->nb32.raw_call = (long)(void*)_klc_keyboard_notifier_cb32;
    }

    return register_keyboard_notifier_fn(nb);
}

static inline int unregister_keyboard_notifier(KLC_NOTIFIER_BLOCK_S *nb)
{
    static int (*unregister_keyboard_notifier_fn)(void *) = NULL;
    if (! unregister_keyboard_notifier_fn) {
        unregister_keyboard_notifier_fn = ulc_sys_get_sym("unregister_keyboard_notifier");
        if (! unregister_keyboard_notifier_fn) {
            return -1;
        }
    }

    return unregister_keyboard_notifier_fn(nb);
}

#ifdef __cplusplus
}
#endif
#endif 
