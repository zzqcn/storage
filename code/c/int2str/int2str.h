#include "common.h"

#ifndef __INT2STR_H__
#define __INT2STR_H__

// clang-format off

/* 话单之间的分隔符 */
#define APPFMT_DELIMITER        '|'
#define APPFMT_DELIMITER_STR    "|"

/*话单中len对应字符串的最大长度，最大不超过此值*/
#define APPFMT_LENGHT_MAX   4
#define XDR_SIZEOF(type)    sizeof(type)

/* 标记数据内存映射字符串的最大值 */
#define APPFMT_IPV4ToBuf_MAX    65536
#define APPFMT_IPV6ToMap_MAX    65536
#define APPFMT_U16MAP_MAX       65536
#define APPFMT_U16To4BMAP_MAX   10000

#define XDR_Memcpy memcpy

// clang-format on

/*1.uint16*/
typedef struct APPFMT_U16MAP
{
    uint8_t StrLen;
    uint64_t Str;
} APPFMT_U16MAP_S;

// /*2.ipV4*/
// typedef struct APPFMT_IPV4MAP
// {
//     uint8_t StrLen;
//     uint64_t Str;
// } APPFMT_IPV4MAP_S;

// /*3.ipV6*/
// typedef struct APPFMT_IPV6MAP
// {
//     uint8_t StrLen;
//     uint32_t Str;
// } APPFMT_IPV6MAP_S;

/* 字符转换接口 */
#define APPFMT_CharToBuf(src, dst) \
    *(dst)++ = (src);              \
    *(dst)++ = APPFMT_DELIMITER;

/* 字符转换接口,srcLen<=0时，会添加分隔符 */
#if 0
#define APPFMT_StrToBuf(src, srcLen, dst)                 \
    if ((srcLen) > 0 && (src) != NULL && (dst) != NULL) { \
        XDR_Memcpy(dst, src, srcLen);                     \
        (dst) += (srcLen);                                \
        *(dst)++ = APPFMT_DELIMITER;                      \
    } else {                                              \
        *(dst)++ = APPFMT_DELIMITER;                      \
    }
#else
#define APPFMT_StrToBuf(src, srcLen, dst) \
    XDR_Memcpy(dst, src, srcLen);         \
    (dst) += (srcLen);                    \
    *(dst)++ = APPFMT_DELIMITER;
#endif

#if 0
#define APPFMT_ShortStrToBuf(src, srcLen, dst)   \
    *((uint64_t *)(dst)) = *((uint64_t *)(src)); \
    (dst) += (srcLen);                           \
    *(dst)++ = APPFMT_DELIMITER;
#else
static inline void _APPFMT_ShortStrToBufNoDelim(const void *src, size_t n, void *dst)
{
    uintptr_t dstu = (uintptr_t)dst;
    uintptr_t srcu = (uintptr_t)src;

    //if (n < 16) {
    if (n & 0x01) {
        *(uint8_t *)dstu = *(const uint8_t *)srcu;
        srcu             = (uintptr_t)((const uint8_t *)srcu + 1);
        dstu             = (uintptr_t)((uint8_t *)dstu + 1);
    }
    if (n & 0x02) {
        *(uint16_t *)dstu = *(const uint16_t *)srcu;
        srcu              = (uintptr_t)((const uint16_t *)srcu + 1);
        dstu              = (uintptr_t)((uint16_t *)dstu + 1);
    }
    if (n & 0x04) {
        *(uint32_t *)dstu = *(const uint32_t *)srcu;
        srcu              = (uintptr_t)((const uint32_t *)srcu + 1);
        dstu              = (uintptr_t)((uint32_t *)dstu + 1);
    }
    if (n & 0x08) {
        *(uint64_t *)dstu = *(const uint64_t *)srcu;
    }
    //   }
}

#define APPFMT_ShortStrToBufNoDelim(src, srcLen, dst) \
    _APPFMT_ShortStrToBufNoDelim(src, srcLen, dst);   \
    (dst) += (srcLen);

#define APPFMT_ShortStrToBuf(src, srcLen, dst)     \
    APPFMT_ShortStrToBufNoDelim(src, srcLen, dst); \
    *(dst)++ = APPFMT_DELIMITER;

#endif

/* uint8转换接口 */
#define APPFMT_U8ToBuf(u16Map, u8, dst)      \
    *((uint64_t *)(dst)) = u16Map[(u8)].Str; \
    (dst) += u16Map[(u8)].StrLen;            \
    *(dst)++ = APPFMT_DELIMITER;

/* uint16转换接口 */
#define APPFMT_U16ToBuf(u16Map, u16, dst)     \
    *((uint64_t *)(dst)) = u16Map[(u16)].Str; \
    (dst) += u16Map[(u16)].StrLen;            \
    *(dst)++ = APPFMT_DELIMITER;

// clang-format off

static const char hexArray[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

// clang-format on

static inline size_t Int2Hex(const char *hex, int num, char *dest)
{
    int a[16] = {0};
    int i     = 0;
    int m     = 0;
    size_t lp = 0;
    int yushu;

    while (num > 0) {
        yushu  = num % 16;
        a[i++] = yushu;
        num    = num / 16;
    }
    for (i = i - 1; i >= 0; i--)  //倒序输出
    {
        m          = a[i];
        dest[lp++] = hex[m];
    }
    return lp;
}

static inline size_t Int2Hex2(int num, char *dst)
{
    return sprintf(dst, "%X", num);
}

extern APPFMT_U16MAP_S g_u16StrMap[APPFMT_U16MAP_MAX];

void InitU16StrMap(void);

#endif
