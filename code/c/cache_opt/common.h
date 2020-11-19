#ifndef __CACHE_COMMON_H__
#define __CACHE_COMMON_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef asm
#define asm __asm__
#endif

/** C extension macro for environments lacking C11 features. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#define RTE_STD_C11 __extension__
#else
#define RTE_STD_C11
#endif

#ifdef __APPLE__
#define FMT64 "%llu"
#else
#define FMT64 "%lu"
#endif


static inline uint64_t rte_rdtsc(void)
{
    union {
        uint64_t tsc_64;
        RTE_STD_C11
            struct {
                uint32_t lo_32;
                uint32_t hi_32;
            };
    } tsc;

    asm volatile("rdtsc" :
            "=a" (tsc.lo_32),
            "=d" (tsc.hi_32));
    return tsc.tsc_64;
}

#ifdef __cplusplus
}
#endif

#endif
