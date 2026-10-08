/*================================================================
*   Created by LiXingang
*
================================================================*/
#ifndef _CPU_DEF_H
#define _CPU_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) || defined(_LP64)
    #define PLATFORM_64BIT 1
    #define PLATFORM_32BIT 0
    #define POINTER_SIZE 8
#else
    #define PLATFORM_64BIT 0
    #define PLATFORM_32BIT 1
    #define POINTER_SIZE 4
#endif

#if ((defined __x86_64__) || (defined __amd64__) || (defined __i386__))
    #define __X86__
#endif

#if ((defined __arm__) || (defined __aarch64__))
    #define __ARM__
    #ifdef __aarch64__
        #define __ARM64__
    #else
        #define __ARM32__
    #endif
#endif

#ifdef __cplusplus
}
#endif
#endif 
