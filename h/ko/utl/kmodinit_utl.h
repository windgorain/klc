/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _K_MODINIT_UTL_H
#define _K_MODINIT_UTL_H

#include "kmodinit_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

static inline int _kmodinit_init(KUTL_MODINIT_S *n)
{
    if (! n->init) {
        return 0;
    }

    return n->init();
}

static inline void _kmodinit_fini(KUTL_MODINIT_S *n)
{
    if (! n->inited) {
        return;
    }

    n->inited = 0;

    if (n->fini) {
        n->fini();
    }
}

static inline KUTL_MODINIT_S * _kmodinit_find_last(KUTL_MODINIT_S *n)
{
    KUTL_MODINIT_S *last = NULL;

    while (n->name) {
        last = n;
        n ++;
    }

    return last;
}

static inline void KModinit_Fini(KUTL_MODINIT_S *init)
{
    KUTL_MODINIT_S *last;
    KUTL_MODINIT_S *n;

    last = _kmodinit_find_last(init);

    
    for (n=last; n>=init; n--) {
        _kmodinit_fini(n);
    }
}

static inline int KModinit_Init(KUTL_MODINIT_S *init)
{
    KUTL_MODINIT_S *n = init;
    int ret = 0;

    for (n=init; n->name; n++) {
        if (_kmodinit_init(n) < 0) {
            break;
        }
        n->inited = 1;
    }

    if (ret < 0) {
        KModinit_Fini(init);
    }

    return ret;
}

#ifdef __cplusplus
}
#endif
#endif 
