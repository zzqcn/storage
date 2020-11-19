/*
 * Copyright (c) 2009-2014 Kazuho Oku, Tokuhiro Matsuno, Daisuke Murase,
 *                         Shigeo Mitsunari
 *
 * The software is licensed under either the MIT License (below) or the Perl
 * license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <assert.h>
#include <stddef.h>
#include <string.h>
#ifdef __AVX2__
#include <immintrin.h>
#elif defined __SSE4_2__
# include <x86intrin.h>
#endif
#include "picohttpparser_avx2.h"

/* $Id: a37bf1827fc2f45abc576de2b7a9a40edd9ed5d2 $ */

#if __GNUC__ >= 3
# define likely(x) __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#else
# define likely(x) (x)
# define unlikely(x) (x)
#endif

#define IS_PRINTABLE_ASCII(c) ((unsigned char)(c) - 040u < 0137u)

#define CHECK_EOF() \
  if (buf == buf_end) { \
    *ret = -2; \
    return NULL; \
  }

#define EXPECT_CHAR(ch) \
  CHECK_EOF(); \
  if (*buf++ != ch) { \
    *ret = -1; \
    return NULL; \
  }

#define ADVANCE_TOKEN(tok, toklen) do { \
    const char* tok_start = buf; \
    static const char ranges2[] __attribute__((aligned(16))) = "\000\040\177\177"; \
    int found2; \
    buf = findchar_fast(buf, buf_end, ranges2, sizeof(ranges2) - 1, &found2); \
    if (! found2) { \
      CHECK_EOF(); \
    } \
    while (1) { \
      if (*buf == ' ') { \
        break; \
      } else if (unlikely(! IS_PRINTABLE_ASCII(*buf))) { \
        if ((unsigned char)*buf < '\040' || *buf == '\177') { \
          *ret = -1; \
          return NULL; \
        } \
      } \
      ++buf; \
      CHECK_EOF(); \
    } \
    tok = tok_start; \
    toklen = buf - tok_start; \
  } while (0)

static const char* token_char_map =
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\1\1\1\1\1\1\1\0\0\1\1\0\1\1\0\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0"
  "\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\1\1"
  "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\0\1\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

static const char* findchar_fast(const char* buf, const char* buf_end, const char *ranges, size_t ranges_size, int* found)
{
  *found = 0;
#if __SSE4_2__
  if (likely(buf_end - buf >= 16)) {
    __m128i ranges16 = _mm_loadu_si128((const __m128i*)ranges);

    size_t left = (buf_end - buf) & ~15;
    do {
      __m128i b16 = _mm_loadu_si128((void*)buf);
      int r = _mm_cmpestri(ranges16, ranges_size, b16, 16, _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS);
      if (unlikely(r != 16)) {
        buf += r;
        *found = 1;
        break;
      }
      buf += 16;
      left -= 16;
    } while (likely(left != 0));
  }
#endif
  return buf;
}

static const char* get_token_to_eol(const char* buf, const char* buf_end,
                                    const char** token, size_t* token_len,
                                    int* ret)
{
  const char* token_start = buf;
  
#ifdef __SSE4_2__
  static const char ranges1[] =
    "\0\010"
    /* allow HT */
    "\012\037"
    /* allow SP and up to but not including DEL */
    "\177\177"
    /* allow chars w. MSB set */
    ;
  int found;
  buf = findchar_fast(buf, buf_end, ranges1, sizeof(ranges1) - 1, &found);
  if (found)
    goto FOUND_CTL;
#else
  /* find non-printable char within the next 8 bytes, this is the hottest code; manually inlined */
  while (likely(buf_end - buf >= 8)) {
#define DOIT() if (unlikely(! IS_PRINTABLE_ASCII(*buf))) goto NonPrintable; ++buf
    DOIT(); DOIT(); DOIT(); DOIT();
    DOIT(); DOIT(); DOIT(); DOIT();
#undef DOIT
    continue;
  NonPrintable:
    if ((likely((unsigned char)*buf < '\040') && likely(*buf != '\011')) || unlikely(*buf == '\177')) {
      goto FOUND_CTL;
    }
    ++buf;
  }
#endif
  for (; ; ++buf) {
    CHECK_EOF();
    if (unlikely(! IS_PRINTABLE_ASCII(*buf))) {
      if ((likely((unsigned char)*buf < '\040') && likely(*buf != '\011')) || unlikely(*buf == '\177')) {
        goto FOUND_CTL;
      }
    }
  }
 FOUND_CTL:
  if (likely(*buf == '\015')) {
    ++buf;
    EXPECT_CHAR('\012');
    *token_len = buf - 2 - token_start;
  } else if (*buf == '\012') {
    *token_len = buf - token_start;
    ++buf;
  } else {
    *ret = -1;
    return NULL;
  }
  *token = token_start;
  
  return buf;
}
  
static const char* is_complete(const char* buf, const char* buf_end,
                               size_t last_len, int* ret)
{
  int ret_cnt = 0;
  buf = last_len < 3 ? buf : buf + last_len - 3;
  
  while (1) {
    CHECK_EOF();
    if (*buf == '\015') {
      ++buf;
      CHECK_EOF();
      EXPECT_CHAR('\012');
      ++ret_cnt;
    } else if (*buf == '\012') {
      ++buf;
      ++ret_cnt;
    } else {
      ++buf;
      ret_cnt = 0;
    }
    if (ret_cnt == 2) {
      return buf;
    }
  }
  
  *ret = -2;
  return NULL;
}

/* *_buf is always within [buf, buf_end) upon success */
static const char* parse_int(const char* buf, const char* buf_end, int* value,
                             int* ret)
{
  int v;
  CHECK_EOF();
  if (! ('0' <= *buf && *buf <= '9')) {
    *ret = -1;
    return NULL;
  }
  v = 0;
  for (; ; ++buf) {
    CHECK_EOF();
    if ('0' <= *buf && *buf <= '9') {
      v = v * 10 + *buf - '0';
    } else {
      break;
    }
  }
  
  *value = v;
  return buf;
}

/* returned pointer is always within [buf, buf_end), or null */
static const char* parse_http_version(const char* buf, const char* buf_end,
                                      int* minor_version, int* ret)
{
  EXPECT_CHAR('H'); EXPECT_CHAR('T'); EXPECT_CHAR('T'); EXPECT_CHAR('P');
  EXPECT_CHAR('/'); EXPECT_CHAR('1'); EXPECT_CHAR('.');
  return parse_int(buf, buf_end, minor_version, ret);
}

#ifdef __AVX2__
static unsigned long TZCNT(unsigned long long in) {
  unsigned long res;
  asm("tzcnt %1, %0\n\t" : "=r"(res) : "r"(in));
  return res;
}
/* Parse only 32 bytes */
static void find_ranges32(__m256i b0, unsigned long *range0, unsigned long *range1) {
  const __m256i rr0 = _mm256_set1_epi8(0x00 - 1);
  const __m256i rr1 = _mm256_set1_epi8(0x1f + 1);
  const __m256i rr2 = _mm256_set1_epi8(0x3a);
  const __m256i rr4 = _mm256_set1_epi8(0x7f);
  const __m256i rr7 = _mm256_set1_epi8(0x09);

  /* 0<=x */
  __m256i gz0 = _mm256_cmpgt_epi8(b0, rr0);
  /* 0=<x<=1f */
  __m256i z_1f_0 = _mm256_and_si256(_mm256_cmpgt_epi8(rr1, b0), gz0);
  /* 0<=x<=1f || x==3a */
  __m256i range0_0 = _mm256_or_si256(_mm256_cmpeq_epi8(rr2, b0), z_1f_0);
  /* 0<=x<9 || 9<x<=1f || x==7f */
  __m256i range1_0 = _mm256_or_si256(_mm256_cmpeq_epi8(rr4, b0), _mm256_andnot_si256(_mm256_cmpeq_epi8(b0, rr7), z_1f_0));
  /* Generate bit masks */
  unsigned int r0 = _mm256_movemask_epi8(range0_0);
  /* Combine 32bit masks into a single 64bit mask */
  *range0 = r0;
  r0 = _mm256_movemask_epi8(range1_0);
  *range1 = r0;
}

/* Parse only 64 bytes */
static void find_ranges64(__m256i b0, __m256i b1, unsigned long *range0, unsigned long *range1) {
  const __m256i rr0 = _mm256_set1_epi8(0x00 - 1);
  const __m256i rr1 = _mm256_set1_epi8(0x1f + 1);
  const __m256i rr2 = _mm256_set1_epi8(0x3a);
  const __m256i rr4 = _mm256_set1_epi8(0x7f);
  const __m256i rr7 = _mm256_set1_epi8(0x09);

  /* 0<=x */
  __m256i gz0 = _mm256_cmpgt_epi8(b0, rr0);
  __m256i gz1 = _mm256_cmpgt_epi8(b1, rr0);
  /* 0=<x<=1f */
  __m256i z_1f_0 = _mm256_and_si256(_mm256_cmpgt_epi8(rr1, b0), gz0);
  __m256i z_1f_1 = _mm256_and_si256(_mm256_cmpgt_epi8(rr1, b1), gz1);
  /* 0<=x<=1f || x==3a */
  __m256i range0_0 = _mm256_or_si256(_mm256_cmpeq_epi8(rr2, b0), z_1f_0);
  __m256i range0_1 = _mm256_or_si256(_mm256_cmpeq_epi8(rr2, b1), z_1f_1);
  /* 0<=x<9 || 9<x<=1f || x==7f */
  __m256i range1_0 = _mm256_or_si256(_mm256_cmpeq_epi8(rr4, b0), _mm256_andnot_si256(_mm256_cmpeq_epi8(b0, rr7), z_1f_0));
  __m256i range1_1 = _mm256_or_si256(_mm256_cmpeq_epi8(rr4, b1), _mm256_andnot_si256(_mm256_cmpeq_epi8(b1, rr7), z_1f_1));
  /* Generate bit masks */
  unsigned int r0 = _mm256_movemask_epi8(range0_0);
  unsigned int r1 = _mm256_movemask_epi8(range0_1);
  /* Combine 32bit masks into a single 64bit mask */
  *range0 = r0 ^ ((unsigned long)r1 << 32);
  r0 = _mm256_movemask_epi8(range1_0);
  r1 = _mm256_movemask_epi8(range1_1);
  *range1 = r0 ^ ((unsigned long)r1 << 32);     
}

/* This function parses 128 bytes at a time, creating bitmap of all interesting tokens */
static void find_ranges(const char* buf, const char* buf_end, unsigned long *range0, unsigned long *range1) {
  const __m256i rr0 = _mm256_set1_epi8(0x00 - 1);
  const __m256i rr1 = _mm256_set1_epi8(0x1f + 1);
  const __m256i rr2 = _mm256_set1_epi8(0x3a);
  const __m256i rr4 = _mm256_set1_epi8(0x7f);
  const __m256i rr7 = _mm256_set1_epi8(0x09);

  __m256i b0, b1, b2, b3;
  unsigned char tmpbuf[32];
  int i;
  int dist;

  if((dist = buf_end - buf) < 128) {
    //memcpy(tmpbuf, buf + (dist & (-32)), dist & 31);
    for (i=0; i < (dist & 31); i++) tmpbuf[i] = buf[ (dist & (-32)) + i];
    if (dist >= 96) {
      b0 = _mm256_loadu_si256((void*) buf + 32*0);
      b1 = _mm256_loadu_si256((void*) buf + 32*1);
      b2 = _mm256_loadu_si256((void*) buf + 32*2);
      b3 = _mm256_loadu_si256((void*) tmpbuf);
    } else if (dist >= 64) {
      b0 = _mm256_loadu_si256((void*) buf + 32*0);
      b1 = _mm256_loadu_si256((void*) buf + 32*1);
      b2 = _mm256_loadu_si256((void*) tmpbuf);
      b3 = _mm256_setzero_si256();
    } else {
      if(dist < 32) {
        b0 = _mm256_loadu_si256((void*)tmpbuf);
        return find_ranges32(b0, range0, range1);
      } else {
        b0 = _mm256_loadu_si256((void*) buf + 32*0);
        b1 = _mm256_loadu_si256((void*)tmpbuf);
        return find_ranges64(b0, b1, range0, range1);
      }
    }
  } else {
    /* Load 128 bytes */
    b0 = _mm256_loadu_si256((void*) buf + 32*0);
    b1 = _mm256_loadu_si256((void*) buf + 32*1);
    b2 = _mm256_loadu_si256((void*) buf + 32*2);
    b3 = _mm256_loadu_si256((void*) buf + 32*3);
  }
 
  /* 0<=x */
  __m256i gz0 = _mm256_cmpgt_epi8(b0, rr0);
  __m256i gz1 = _mm256_cmpgt_epi8(b1, rr0);
  __m256i gz2 = _mm256_cmpgt_epi8(b2, rr0);
  __m256i gz3 = _mm256_cmpgt_epi8(b3, rr0);
  /* 0=<x<=1f */
  __m256i z_1f_0 = _mm256_and_si256(_mm256_cmpgt_epi8(rr1, b0), gz0);
  __m256i z_1f_1 = _mm256_and_si256(_mm256_cmpgt_epi8(rr1, b1), gz1);
  __m256i z_1f_2 = _mm256_and_si256(_mm256_cmpgt_epi8(rr1, b2), gz2);
  __m256i z_1f_3 = _mm256_and_si256(_mm256_cmpgt_epi8(rr1, b3), gz3);
  /* 0<=x<=1f || x==3a */
  __m256i range0_0 = _mm256_or_si256(_mm256_cmpeq_epi8(rr2, b0), z_1f_0);
  __m256i range0_1 = _mm256_or_si256(_mm256_cmpeq_epi8(rr2, b1), z_1f_1);
  __m256i range0_2 = _mm256_or_si256(_mm256_cmpeq_epi8(rr2, b2), z_1f_2);
  __m256i range0_3 = _mm256_or_si256(_mm256_cmpeq_epi8(rr2, b3), z_1f_3);
  /* 0<=x<9 || 9<x<=1f || x==7f */
  __m256i range1_0 = _mm256_or_si256(_mm256_cmpeq_epi8(rr4, b0), _mm256_andnot_si256(_mm256_cmpeq_epi8(b0, rr7), z_1f_0));
  __m256i range1_1 = _mm256_or_si256(_mm256_cmpeq_epi8(rr4, b1), _mm256_andnot_si256(_mm256_cmpeq_epi8(b1, rr7), z_1f_1));
  __m256i range1_2 = _mm256_or_si256(_mm256_cmpeq_epi8(rr4, b2), _mm256_andnot_si256(_mm256_cmpeq_epi8(b2, rr7), z_1f_2));
  __m256i range1_3 = _mm256_or_si256(_mm256_cmpeq_epi8(rr4, b3), _mm256_andnot_si256(_mm256_cmpeq_epi8(b3, rr7), z_1f_3));
  /* Generate bit masks */
  unsigned int r0 = _mm256_movemask_epi8(range0_0);
  unsigned int r1 = _mm256_movemask_epi8(range0_1);
  /* Combine 32bit masks into a single 64bit mask */
  *range0 = r0 ^ ((unsigned long)r1 << 32);

  r0 = _mm256_movemask_epi8(range0_2);
  r1 = _mm256_movemask_epi8(range0_3);
  range0[1] = r0 ^ ((unsigned long)r1 << 32);

  r0 = _mm256_movemask_epi8(range1_0);
  r1 = _mm256_movemask_epi8(range1_1);

  *range1 = r0 ^ ((unsigned long)r1 << 32);     
  r0 = _mm256_movemask_epi8(range1_2);
  r1 = _mm256_movemask_epi8(range1_3);

  range1[1] = r0 ^ ((unsigned long)r1 << 32);
}

static const char* parse_headers(const char* buf, const char* buf_end, 
                                 struct phr_header* headers,
                                 size_t* num_headers, size_t max_headers,
                                 int* ret)
{
  /* Bitmap for the first type of tokens */
  unsigned long rr0[2] = {0};
  /* Bitmap for the second type of tokens */
  unsigned long rr1[2] = {0};
  /* Pointer to the start of the currently parsed block of 128 bytes */
  const char* prep_start = NULL;
  int found;
  int n_headers = *num_headers;

  for (; ; ++n_headers) {
    CHECK_EOF();
    if (*buf == '\015') {
      ++buf;
      EXPECT_CHAR('\012');
      break;
    } else if (*buf == '\012') {
      ++buf;
      break;
    }
    if (n_headers == max_headers) {
      *ret = -1;
      *num_headers = n_headers;
      return NULL;
    }

    // zzqcn
    //if (! (n_headers != 0 && (*buf == ' ' || *buf == '\t')) && !(*buf >= 65 && * buf <= 90)) {
    if (! (n_headers != 0 && (*buf == ' ' || *buf == '\t'))) {
      if (! token_char_map[(unsigned char)*buf]) {
        *ret = -1;
        *num_headers = n_headers;
        return NULL;
      }
      headers[n_headers].name = buf;
 
      /* Attempt to find a match in the index */
      found = 0;
      do {
        unsigned long distance = buf - prep_start;
        /* Check if the bitmaps are still valid. An assumption I make is that 
           buf > 128 (i.e. the os will never allocate memory at address 0-128 */
        if(unlikely(distance >= 128)) {   /* Bitmaps are too old, make new ones */
          prep_start = buf;
          distance = 0;
          find_ranges(buf, buf_end, rr0, rr1);
        } else if(distance >= 64) {          /* In the second half of the bitmap */
          unsigned long index = rr0[1] >> (distance - 64);  /* Correct offset of the bitmap */
          unsigned long find = TZCNT(index); /* Fine next set bit */
          if((find < 64)) {                  /* Yey, we found a token */
            buf += find;
            found = 1;
            break;
          }
          buf = prep_start + 128;            /* No token was found in the current bitmap */
          continue;
        }
        unsigned long index = rr0[0]  >> (distance); /* In the first half of the bitmap */
        unsigned long find = TZCNT(index);           /* Find next set bit */
        if((find < 64)){                             /* Token found */
          buf += find;
          found = 1;
          break;
        }                                            /* Token not found, look at second half of bitmap */
        index = rr0[1];
        find = TZCNT(index);
        if((find < 64)){
          buf += 64+find - distance;
          found = 1;
          break;
        }     

        buf = prep_start + 128;
      } while (buf < buf_end);
      
      if(!found)
        if(buf >= buf_end) {
          *ret = -2;
          *num_headers = n_headers;
          return NULL;
        }
      headers[n_headers].name_len = buf - headers[n_headers].name;
      ++buf;
      CHECK_EOF();
      while( (*buf == ' ' || *buf == '\t') ) {
        ++buf;
        CHECK_EOF();
      }
    } else {
      headers[n_headers].name = NULL;
      headers[n_headers].name_len = 0;
    }
    const char* token_start = buf;

    found = 0;

    do {
      /* Too far */
      unsigned long distance = buf - prep_start; /* Same algorithm as above */
      if(unlikely(distance >= 128)) {
        prep_start = buf;
        distance = 0;
        find_ranges(buf, buf_end, rr0, rr1);
      } else if(distance >= 64) {
        unsigned long index = rr1[1] >> (distance - 64);
        unsigned long find = TZCNT(index);
        if((find < 64)) {
          buf += find;
          found = 1;
          break;
        }
        buf = prep_start + 128;
        continue;
      }
      unsigned long index = rr1[0]  >> (distance);
      unsigned long find = TZCNT(index);
      if((find < 64)){
        buf += find;
        found = 1;
        break;
      }
      index = rr1[1];
      find = TZCNT(index);
      if((find < 64)){
        buf += 64+find - distance;
        found = 1;
        break;
      }
        
      buf = prep_start + 128;
    } while (buf < buf_end);
 
    if(!found)
      if(buf >= buf_end) {
        *ret = -2;
        *num_headers = n_headers;
        return NULL;
      }

    unsigned short two_char = *(unsigned short*)buf;

    if( likely(two_char == 0x0a0d) ) {
      headers[n_headers].value_len = buf - token_start;
      buf += 2;
    } else if (unlikely(two_char & 0x0a == 0x0a)) {
      headers[n_headers].value_len = buf - token_start;
      ++buf;
    } else {
      *ret = -1;
      *num_headers = n_headers;
      return NULL;
    }
    headers[n_headers].value = token_start;
  }
  *num_headers = n_headers;
  return buf;
}
#else

static const char* parse_headers(const char* buf, const char* buf_end,
                                 struct phr_header* headers,
                                 size_t* num_headers, size_t max_headers,
                                 int* ret)
{
  for (; ; ++*num_headers) {
    CHECK_EOF();
    if (*buf == '\015') {
      ++buf;
      EXPECT_CHAR('\012');
      break;
    } else if (*buf == '\012') {
      ++buf;
      break;
    }
    if (*num_headers == max_headers) {
      *ret = -1;
      return NULL;
    }
    if (! (*num_headers != 0 && (*buf == ' ' || *buf == '\t'))) {
      if (! token_char_map[(unsigned char)*buf]) {
        *ret = -1;
        return NULL;
      }
      /* parsing name, but do not discard SP before colon, see
       * http://www.mozilla.org/security/announce/2006/mfsa2006-33.html */
      headers[*num_headers].name = buf;
      static const char ranges1[] __attribute__((aligned(16))) = "::\x00\037";
      int found;
      buf = findchar_fast(buf, buf_end, ranges1, sizeof(ranges1) - 1, &found);
      if (! found) {
        CHECK_EOF();
      }
      while (1) {
        if (*buf == ':') {
          break;
        } else if (*buf < ' ') {
          *ret = -1;
          return NULL;
        }
        ++buf;
        CHECK_EOF();
      }
      headers[*num_headers].name_len = buf - headers[*num_headers].name;
      ++buf;
      for (; ; ++buf) {
        CHECK_EOF();
        if (! (*buf == ' ' || *buf == '\t')) {
          break;
        }
      }
    } else {
      headers[*num_headers].name = NULL;
      headers[*num_headers].name_len = 0;
    }
    if ((buf = get_token_to_eol(buf, buf_end, &headers[*num_headers].value,
                                &headers[*num_headers].value_len, ret))
        == NULL) {
      return NULL;
    }
  }
  return buf;
}
#endif

static const char* parse_request(const char* buf, const char* buf_end,
                                 const char** method, size_t* method_len,
                                 const char** path, size_t* path_len,
                                 int* minor_version, struct phr_header* headers,
                                 size_t* num_headers, size_t max_headers,
                                 int* ret)
{
  /* skip first empty line (some clients add CRLF after POST content) */
  CHECK_EOF();
  if (*buf == '\015') {
    ++buf;
    EXPECT_CHAR('\012');
  } else if (*buf == '\012') {
    ++buf;
  }
  
  /* parse request line */
  ADVANCE_TOKEN(*method, *method_len);
  ++buf;
  ADVANCE_TOKEN(*path, *path_len);
  ++buf;
  if ((buf = parse_http_version(buf, buf_end, minor_version, ret)) == NULL) {
    return NULL;
  }
  if (*buf == '\015') {
    ++buf;
    EXPECT_CHAR('\012');
  } else if (*buf == '\012') {
    ++buf;
  } else {
    *ret = -1;
    return NULL;
  }
  
  return parse_headers(buf, buf_end, headers, num_headers, max_headers, ret);
}

int phr_parse_request(const char* buf_start, size_t len, const char** method,
                      size_t* method_len, const char** path, size_t* path_len,
                      int* minor_version, struct phr_header* headers,
                      size_t* num_headers, size_t last_len)
{
  const char * buf = buf_start, * buf_end = buf_start + len;
  size_t max_headers = *num_headers;
  int r;
  
  *method = NULL;
  *method_len = 0;
  *path = NULL;
  *path_len = 0;
  *minor_version = -1;
  *num_headers = 0;
  
  /* if last_len != 0, check if the request is complete (a fast countermeasure
     againt slowloris */
  if (last_len != 0 && is_complete(buf, buf_end, last_len, &r) == NULL) {
    return r;
  }
  
  if ((buf = parse_request(buf, buf_end, method, method_len, path, path_len,
                           minor_version, headers, num_headers, max_headers,
                           &r))
      == NULL) {
    return r;
  }
  
  return (int)(buf - buf_start);
}

static const char* parse_response(const char* buf, const char* buf_end,
                                  int* minor_version, int* status,
                                  const char** msg, size_t* msg_len,
                                  struct phr_header* headers,
                                  size_t* num_headers, size_t max_headers,
                                  int* ret)
{
  /* parse "HTTP/1.x" */
  if ((buf = parse_http_version(buf, buf_end, minor_version, ret)) == NULL) {
    return NULL;
  }
  /* skip space */
  if (*buf++ != ' ') {
    *ret = -1;
    return NULL;
  }
  /* parse status code */
  if ((buf = parse_int(buf, buf_end, status, ret)) == NULL) {
    return NULL;
  }
  /* skip space */
  if (*buf++ != ' ') {
    *ret = -1;
    return NULL;
  }
  /* get message */
  if ((buf = get_token_to_eol(buf, buf_end, msg, msg_len, ret)) == NULL) {
    return NULL;
  }
  
  return parse_headers(buf, buf_end, headers, num_headers, max_headers, ret);
}

int phr_parse_response(const char* buf_start, size_t len, int* minor_version,
                       int* status, const char** msg, size_t* msg_len,
                       struct phr_header* headers, size_t* num_headers,
                       size_t last_len)
{
  const char * buf = buf_start, * buf_end = buf + len;
  size_t max_headers = *num_headers;
  int r;
  
  *minor_version = -1;
  *status = 0;
  *msg = NULL;
  *msg_len = 0;
  *num_headers = 0;
  
  /* if last_len != 0, check if the response is complete (a fast countermeasure
     against slowloris */
  if (last_len != 0 && is_complete(buf, buf_end, last_len, &r) == NULL) {
    return r;
  }
  
  if ((buf = parse_response(buf, buf_end, minor_version, status, msg, msg_len,
                            headers, num_headers, max_headers, &r))
      == NULL) {
    return r;
  }
  
  return (int)(buf - buf_start);
}

int phr_parse_headers(const char* buf_start, size_t len,
                      struct phr_header* headers, size_t* num_headers,
                      size_t last_len)
{
  const char* buf = buf_start, * buf_end = buf + len;
  size_t max_headers = *num_headers;
  int r;

  *num_headers = 0;

  /* if last_len != 0, check if the response is complete (a fast countermeasure
     against slowloris */
  if (last_len != 0 && is_complete(buf, buf_end, last_len, &r) == NULL) {
    return r;
  }

  if ((buf = parse_headers(buf, buf_end, headers, num_headers, max_headers, &r))
      == NULL) {
    return r;
  }

  return (int)(buf - buf_start);
}

enum {
  CHUNKED_IN_CHUNK_SIZE,
  CHUNKED_IN_CHUNK_EXT,
  CHUNKED_IN_CHUNK_DATA,
  CHUNKED_IN_CHUNK_CRLF,
  CHUNKED_IN_TRAILERS_LINE_HEAD,
  CHUNKED_IN_TRAILERS_LINE_MIDDLE
};

static int decode_hex(int ch)
{
  if ('0' <= ch && ch <= '9') {
    return ch - '0';
  } else if ('A' <= ch && ch <= 'F') {
    return ch - 'A' + 0xa;
  } else if ('a' <= ch && ch <= 'f') {
    return ch - 'a' + 0xa;
  } else {
    return -1;
  }
}

ssize_t phr_decode_chunked(struct phr_chunked_decoder *decoder, char *buf,
                           size_t *_bufsz)
{
  size_t dst = 0, src = 0, bufsz = *_bufsz;
  ssize_t ret = -2; /* incomplete */

  while (1) {
    switch (decoder->_state) {
    case CHUNKED_IN_CHUNK_SIZE:
      for (; ; ++src) {
        int v;
        if (src == bufsz)
          goto Exit;
        if ((v = decode_hex(buf[src])) == -1) {
          if (decoder->_hex_count == 0) {
            ret = -1;
            goto Exit;
          }
          break;
        }
        if (decoder->_hex_count == sizeof(size_t) * 2) {
          ret = -1;
          goto Exit;
        }
        decoder->bytes_left_in_chunk = decoder->bytes_left_in_chunk * 16 + v;
        ++decoder->_hex_count;
      }
      decoder->_hex_count = 0;
      decoder->_state = CHUNKED_IN_CHUNK_EXT;
      /* fallthru */
    case CHUNKED_IN_CHUNK_EXT:
      /* RFC 7230 A.2 "Line folding in chunk extensions is disallowed" */
      for (; ; ++src) {
        if (src == bufsz)
          goto Exit;
        if (buf[src] == '\012')
          break;
      }
      ++src;
      if (decoder->bytes_left_in_chunk == 0) {
        if (decoder->consume_trailer) {
          decoder->_state = CHUNKED_IN_TRAILERS_LINE_HEAD;
          break;
        } else {
          goto Complete;
        }
      }
      decoder->_state = CHUNKED_IN_CHUNK_DATA;
      /* fallthru */
    case CHUNKED_IN_CHUNK_DATA:
      {
        size_t avail = bufsz - src;
        if (avail < decoder->bytes_left_in_chunk) {
          if (dst != src)
            memmove(buf + dst, buf + src, avail);
          src += avail;
          dst += avail;
          decoder->bytes_left_in_chunk -= avail;
          goto Exit;
        }
        if (dst != src)
          memmove(buf + dst, buf + src, decoder->bytes_left_in_chunk);
        src += decoder->bytes_left_in_chunk;
        dst += decoder->bytes_left_in_chunk;
        decoder->bytes_left_in_chunk = 0;
        decoder->_state = CHUNKED_IN_CHUNK_CRLF;
      }
      /* fallthru */
    case CHUNKED_IN_CHUNK_CRLF:
      for (; ; ++src) {
        if (src == bufsz)
          goto Exit;
        if (buf[src] != '\015')
          break;
      }
      if (buf[src] != '\012') {
        ret = -1;
        goto Exit;
      }
      ++src;
      decoder->_state = CHUNKED_IN_CHUNK_SIZE;
      break;
    case CHUNKED_IN_TRAILERS_LINE_HEAD:
      for (; ; ++src) {
        if (src == bufsz)
          goto Exit;
        if (buf[src] != '\015')
          break;
      }
      if (buf[src++] == '\012')
        goto Complete;
      decoder->_state = CHUNKED_IN_TRAILERS_LINE_MIDDLE;
      /* fallthru */
    case CHUNKED_IN_TRAILERS_LINE_MIDDLE:
      for (; ; ++src) {
        if (src == bufsz)
          goto Exit;
        if (buf[src] == '\012')
          break;
      }
      ++src;
      decoder->_state = CHUNKED_IN_TRAILERS_LINE_HEAD;
      break;
    default:
      assert(!"decoder is corrupt");
    }
  }

Complete:
  ret = bufsz - src;
Exit:
  if (dst != src)
    memmove(buf + dst, buf + src, bufsz - src);
  *_bufsz = dst;
  return ret;
}

#undef CHECK_EOF
#undef EXPECT_CHAR
#undef ADVANCE_TOKEN
