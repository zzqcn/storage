#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#ifndef __SSE4_2__
#error SSE4.2 is required!
#endif
#include <x86intrin.h>

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

char *myStrstr(const char *haystack, const char *needle)
{
    int ret;
    __m128i ptn = _mm_loadu_si128((const __m128i *)needle);
    __m128i b16 = _mm_loadu_si128((const __m128i *)haystack);

    ret = _mm_cmpistri(ptn, b16, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_LEAST_SIGNIFICANT);

    if(ret == 16)
        return NULL;

    return (char *)haystack + ret;
}

#ifdef DEBUG
#define LOOP_COUNT 1
#else
#define LOOP_COUNT 1000000
#endif

char *ret;

int main(int argc, char **argv)
{
    const char *str, *ptn;
    uint64_t start, stop;
    int i;

    if (argc < 3) {
        printf("usage: %s <string> <pattern>\n", argv[0]);
        return -1;
    }
    str = argv[1];
    ptn = argv[2];

    start = rte_rdtsc();
    for (i = 0; i < LOOP_COUNT; i++) {
        ret = strstr(str, ptn);
#ifdef DEBUG
        printf("ret=%s\n", ret);
#endif
    }
    stop = rte_rdtsc();

    printf(FMT64 ", ", stop - start);

    start = rte_rdtsc();
    for (i = 0; i < LOOP_COUNT; i++) {
        ret = myStrstr(str, ptn);
#ifdef DEBUG
        printf("ret=%s\n", ret);
#endif
    }
    stop = rte_rdtsc();

    printf(FMT64 "\n", stop - start);

    return 0;
}
