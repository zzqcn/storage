/**
 * @file
 * @author zzqcn
 * @brief setjmp/longjmp极简示例
 * @see https://www.cnblogs.com/hazir/p/c_setjmp_longjmp.html
 * @date 2021.09.09
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

int main(int argc, char** argv)
{
    jmp_buf env;
    int n;

    n = setjmp(env);
    printf("n: %d\n", n);
    if (n != 0)
        exit(0);

    longjmp(env, 2);
    printf("This line does not get printed\n");

    return 0;
}
