/**
 * 将表示数字范围的字符串解析为数组
 *
 * 语法见main函数中的test.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int get_ids(const char* s, int* arr, int arr_len, int* len)
{
    int ret, i = 0, index = 0;
    int min, max;
    char *c, *c2;

    c = c2 = strdup(s);
    do {
        c = strtok(c, ",");
        if(NULL == c)
            break;
        if(strchr(c, '-')) {
            if(strrchr(c, '-') != strchr(c, '-'))
                goto fail;
            ret = sscanf(c, "%u-%u", &min, &max);
            if(ret != 2 || min >= max)
                goto fail;
            for(i=min; i<=max; i++) {
                if((index+1) > arr_len)
                    goto fail;
                arr[index++] = i;
            }
        } else {
            if((index+1) > arr_len)
                goto fail;
            ret = sscanf(c, "%u", &arr[index++]);
            if(ret != 1)
                goto fail;
        }
        c = NULL;
    } while(1);

    free(c2);
    *len = index;
    return 0;

fail:
    free(c2);
    *len = 0;
    return -1;
}

#define N 32

#define TEST(x) do { \
    if((x) != 0) \
    printf("FAILED: %s\n", #x); \
    else { \
        for(i=0; i<len; i++) \
        printf("%u ", arr[i]); \
        printf("\n"); \
    } \
} while(0)

int main(void)
{
    int i, arr[N], len;

    TEST(get_ids("2", arr, N, &len));
    TEST(get_ids("2,3,4", arr, N, &len));
    TEST(get_ids("2-8", arr, N, &len));
    TEST(get_ids("2,3-8", arr, N, &len));
    TEST(get_ids("2,3-8,9", arr, N, &len));
    TEST(get_ids("2-8,9", arr, N, &len));
    TEST(get_ids("2-8,9,10-16", arr, N, &len));

    TEST(get_ids("2,", arr, N, &len));
    TEST(get_ids(",2,3,4", arr, N, &len));
    TEST(get_ids("-8", arr, N, &len));
    TEST(get_ids("2,3-8,", arr, N, &len));
    TEST(get_ids("3-,9", arr, N, &len));
    TEST(get_ids("2-8,9-", arr, N, &len));
    TEST(get_ids("2-8-9,10,11-16", arr, N, &len));

    return 0;
}
