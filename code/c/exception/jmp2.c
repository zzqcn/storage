/**
 * @file
 * @author zzqcn
 * @brief setjmp/longjmp示例
 * @see https://www.cnblogs.com/hazir/p/c_setjmp_longjmp.html
 * @date 2021.09.09
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

double divide(double to, double by)
{
    if (by == 0)
        longjmp(env, 1);
    return to / by;
}

void f()
{
    if (setjmp(env) == 0)
        divide(2, 0);
    else
        printf("Cannot / 0\n");
    printf("done\n");
}

int main(int argc, char** argv)
{
    f();

    return 0;
}
