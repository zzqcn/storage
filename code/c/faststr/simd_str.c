#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#ifndef __SSE4_2__
#error SSE4.2 is required!
#endif
#include <x86intrin.h>

#if __GNUC__ >= 3
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#define ALIGNED(n) __attribute__((aligned(n)))

#ifdef __APPLE__
#define FMT64u  "%llu"
#define FMT64i  "%lld"
#else
#define FMT64u "%lu"
#define FMT64i "%ld"
#endif

int str_cmpi_cmp(const char *a, const char *b)
{
    int r;
    __m128i a16 = _mm_loadu_si128((const __m128i *)a);
    __m128i b16 = _mm_loadu_si128((const __m128i *)b);

    r = _mm_cmpistri(
        a16, b16,
        _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_LEAST_SIGNIFICANT);

    printf("result: %d\n", r);

    return r;
}

// void print_uint64_bits(uint64_t x)
// {
//     int i;
//     for(i=0; i<64; i++) {
//         if( (x << i) & (1 << i) )
//             printf("1");
//         else
//             printf("0");
//     }
//     printf("\n");
// }

int str_cmpi_cmp_mask(const char *a, const char *b)
{
    uint64_t r;
    __m128i r16;
    __m128i a16 = _mm_loadu_si128((const __m128i *)a);
    __m128i b16 = _mm_loadu_si128((const __m128i *)b);

    r16 = _mm_cmpistrm(
        a16, b16,
        _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_LEAST_SIGNIFICANT | _SIDD_BIT_MASK);

    r =  (uint64_t)_mm_extract_epi64(r16, 0);
    printf("mask: %0llx\n", r);

    return r;
}

int str_cmpi_set(const char *set, const char *str)
{
    int r;
    __m128i set16 = _mm_loadu_si128((const __m128i *)set);
    __m128i b16 = _mm_loadu_si128((const __m128i *)str);

    r = _mm_cmpistri(set16, b16, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_LEAST_SIGNIFICANT);

    if (r != 16)
        printf("found at: %d\n", r);
    else
        printf("not found: %d\n", r);

    return r;
}

int str_cmpi_range(const char *ranges, const char *str)
{
    int r = -1;

    __m128i ranges16 = _mm_loadu_si128((const __m128i *)ranges);
    __m128i b16 = _mm_loadu_si128((const __m128i *)str);
    r = _mm_cmpistri(ranges16, b16, _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS);

    if (r != 16)
        printf("found at: %d\n", r);
    else
        printf("not found: %d\n", r);

    return r;
}

int str_cmpe_range(const char *ranges, const char *str)
{
    int r = -1;

    __m128i ranges16 = _mm_loadu_si128((const __m128i *)ranges);
    __m128i b16 = _mm_loadu_si128((const __m128i *)str);
    r = _mm_cmpestri(ranges16, strlen(ranges), b16, strlen(str),
                     _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS);

    if (r != 16)
        printf("found at: %d\n", r);
    else
        printf("not found: %d\n", r);

    return r;
}

int str_cmpi_substr(const char *ptn, const char *str)
{
    int r;
    __m128i ptn16 = _mm_loadu_si128((const __m128i *)ptn);
    __m128i b16 = _mm_loadu_si128((const __m128i *)str);

    r = _mm_cmpistri(ptn16, b16,
                     _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_LEAST_SIGNIFICANT);

    if (r != 16)
        printf("found at: %d\n", r);
    else
        printf("not found: %d\n", r);

    return r;
}

#define CALL_STR_FUNC(func, a, b)                    \
    do {                                             \
        printf("str_%s: '%s', '%s', ", #func, a, b); \
        str_##func(a, b);                            \
    } while (0)

int main(void)
{
    int ret;

    // strcmp
    // CALL_STR_FUNC(cmpi_cmp, "abcdefg", "abcdefg");
    // CALL_STR_FUNC(cmpi_cmp, "abcdefg", "Abcdefg");
    // CALL_STR_FUNC(cmpi_cmp, "abcdefg", "ABCDefg");
    // CALL_STR_FUNC(cmpi_cmp, "abcdefg", "abCDEFG");
    // CALL_STR_FUNC(cmpi_cmp, "abcdefg", "abc#efg");
    // CALL_STR_FUNC(cmpi_cmp, "abcdefg", "##aBCDefg");
    // CALL_STR_FUNC(cmpi_cmp, "abcdefg", "abcdefghijk");

    CALL_STR_FUNC(cmpi_cmp_mask, "abcdefg", "abcdefg");
    CALL_STR_FUNC(cmpi_cmp_mask, "abcdefg", "Abcdefg");
    CALL_STR_FUNC(cmpi_cmp_mask, "abcdefg", "ABCDefg");
    CALL_STR_FUNC(cmpi_cmp_mask, "abcdefg", "abCDEFG");
    CALL_STR_FUNC(cmpi_cmp_mask, "abcdefg", "abc#efg");
    CALL_STR_FUNC(cmpi_cmp_mask, "abcdefg", "##aBCDefg");
    CALL_STR_FUNC(cmpi_cmp_mask, "abcdefg", "abcdefghijk");

    // strspn
    CALL_STR_FUNC(cmpi_set, "abc", "##a####b###c#");
    CALL_STR_FUNC(cmpi_set, "abc", "####a#bc#####");
    CALL_STR_FUNC(cmpi_set, "abc", "#############");

    // range
    CALL_STR_FUNC(cmpi_range, "09az", "######x##");
    CALL_STR_FUNC(cmpi_range, "09az", "ABCDEFGHIJKLMN");
    CALL_STR_FUNC(cmpe_range, "09az", "######x##");
    CALL_STR_FUNC(cmpe_range, "09az", "ABCDEFGHIJKLMN");

    // strstr
    CALL_STR_FUNC(cmpi_substr, "abc", "######abc####");
    CALL_STR_FUNC(cmpi_substr, "abc", "####a#bc#####");
    CALL_STR_FUNC(cmpi_substr, "abc", "#############");

    return 0;
}
