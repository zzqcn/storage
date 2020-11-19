#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __COMMON_H__
#define __COMMON_H__

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

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif /* likely */

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif /* unlikely */

#define RTE_CACHE_LINE_SIZE 64
#define __rte_aligned(a) __attribute__((__aligned__(a)))
#define __rte_packed __attribute__((__packed__))
#define __rte_cache_aligned __rte_aligned(RTE_CACHE_LINE_SIZE)

static inline uint64_t rte_rdtsc(void)
{
    union
    {
        uint64_t tsc_64;
        RTE_STD_C11
        struct
        {
            uint32_t lo_32;
            uint32_t hi_32;
        };
    } tsc;

    asm volatile("rdtsc" : "=a"(tsc.lo_32), "=d"(tsc.hi_32));
    return tsc.tsc_64;
}

#endif