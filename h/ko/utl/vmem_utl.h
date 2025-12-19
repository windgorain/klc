/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _VMEM_UTL_H
#define _VMEM_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

static inline void * VMEM_ZMalloc(unsigned int size)
{
    void *m = vmalloc(size);
    if (m) {
        memset(m, 0, size);
    }
    return m;
}

static inline void VMEM_Free(void *mem)
{
    if (mem) {
        vfree(mem);
    }
}

static inline void * VMEM_Dup(void *buf, unsigned int size)
{
    void *m = vmalloc(size);
    if (m) {
        memcpy(m, buf, size);
    }
    return m;
}

#ifdef __cplusplus
}
#endif
#endif 
