#include <assert.h>
#include <stddef.h>
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


static const char *findchar_fast(const char *buf, const char *buf_end, const char *ranges,
                                 size_t ranges_size, int *found)
{
    *found = 0;

    if (likely(buf_end - buf >= 16)) {
        __m128i ranges16 = _mm_loadu_si128((const __m128i *)ranges);

        size_t left = (buf_end - buf) & ~15;
        do {
            __m128i b16 = _mm_loadu_si128((const __m128i *)buf);
            int r = _mm_cmpestri(ranges16, ranges_size, b16, 16,
                                 _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS);
            if (unlikely(r != 16)) {
                printf("r=%d\n", r);
                buf += r;
                *found = 1;
                break;
            }
            buf += 16;
            left -= 16;
        } while (likely(left != 0));
    }

    return buf;
}

int main(void)
{
    const char* p;
    int found;
    char* str = "abc}cccc=cccccccccccccccccccccccccccccccccccccccccccccccc";
    char* str_end = str + strlen(str);
    static const char ALIGNED(16) ranges1[] = "\x00 "  /* control chars and up to SP */
                                              "\"\""   /* 0x22 */
                                              "()"     /* 0x28,0x29 */
                                              ",,"     /* 0x2c */
                                              "//"     /* 0x2f */
                                              ":@"     /* 0x3a-0x40 */
                                              "[]"     /* 0x5b-0x5d */
                                              "{\377"; /* 0x7b-0xff */

    found = 0;
    p = findchar_fast(str, str_end, ranges1, sizeof(ranges1)-1, &found);

    printf("p=%s, found=%d\n", p, found);

    return 0;
}