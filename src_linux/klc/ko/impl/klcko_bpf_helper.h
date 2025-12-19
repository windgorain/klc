/******************************************************************
*   Created by LiXingang
*   Description: 
*
******************************************************************/
#ifndef _KLCKO_BPF_HELPER_H
#define _KLCKO_BPF_HELPER_H
#ifdef __cplusplus
extern "C"
{
#endif

const void ** ulc_get_base_helpers(void);
const void ** ulc_get_sys_helpers(void);
const void ** ulc_get_user_helpers(void);
void * ulc_get_helper(unsigned int id, const void **tmp_helpers);

int KlcKoHelper_Init(void);
void KlcKoHelper_Fini(void);

#ifdef __cplusplus
}
#endif
#endif 
