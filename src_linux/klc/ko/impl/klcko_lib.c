/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "klcko_impl.h"
#include "linux/gfp.h"

int KlcKo_Init(void)
{
    KlcKo_BaseEnable(1);
    return 0;
}

void KlcKo_Fini(void)
{
    int i;

    KlcKo_BaseEnable(0);

    for (i=0; i<KLCKO_BASE_FUNC_NUM; i++) {
        KlcKo_SetBaseFunc(i, NULL);
    }
}

