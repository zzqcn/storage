/**
 * @file
 * @author zzqcn
 * @brief setjmp/longjmp示例2
 * @see https://www.cnblogs.com/hazir/p/c_setjmp_longjmp.html
 * @date 2021.09.09
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;

/**
 * 可抛出异常的函数.
 *
 * 根据arg的值决定正常处理还是抛出异常:
 * - arg==0: 抛出异常1
 * - arg<0: 抛出异常2
 * - arg>5: 抛出异常3
 * - arg为其他值: 正常处理
 *
 * @param arg 参数
 */
void foo(int arg)
{
    if (arg == 0)
        longjmp(env, 1);
    else if (arg < 0)
        longjmp(env, 2);
    else if (arg > 5)
        longjmp(env, 3);

    printf("foo: normal\n");
}

int main(int argc, char** argv)
{
    int arg;
    int exception;

    if (argc < 2)
        arg = 0;
    else
        arg = atoi(argv[1]);

    exception = setjmp(env);
    /*
     * 根据setjmp返回值进行异常处理.
     *
     * - 0: 直接调用setjmp返回
     * - 1: 捕获并处理异常1; 要注意longjmp调用第2个参数为0时, setjmp也返回1
     * - 2: 捕获并处理异常2
     * - 其他值: 未捕获的其他异常
     */
    switch (exception) {
    case 0:
        // 当arg值正常, foo函数正常返回时, 由于没有调用longjmp,
        // main函数会正常结束,
        // 否则跳转到setjmp调用那一行执行
        foo(arg);
        break;
    case 1:
        printf("exception 1\n");
        exit(EXIT_FAILURE);
    case 2:
        printf("exception 2\n");
        exit(EXIT_FAILURE);
    default:
        printf("unknown exception %d\n", exception);
        exit(EXIT_FAILURE);
    }

    printf("main end\n");

    return 0;
}
