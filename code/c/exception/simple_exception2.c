/**
 * @file
 * @author zzqcn
 * @brief 基于setjmp/longjmp实现的极简异常处理, 添加了finally语句
 * @see http://groups.di.unipi.it/~nids/docs/longjump_try_trow_catch.html
 * @date 2021.09.09
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;

// clang-format off
#define TRY do { switch (setjmp(env)) { case 0: while (1) {
#define CATCH(x) break; case x:
#define FINALLY break; } default: {
#define ENDTRY break; } } } while (0)
#define THROW(x) longjmp(env, x)

#define FOO_EXCEPTION (1)
#define BAR_EXCEPTION (2)
#define BAZ_EXCEPTION (3)
// clang-format on

void foo(int arg)
{
    if (arg == 0)
        THROW(FOO_EXCEPTION);
    else if (arg < 0)
        THROW(BAR_EXCEPTION);
    else if (arg > 5)
        THROW(BAZ_EXCEPTION);

    printf("foo: normal\n");
}

int main(int argc, char** argv)
{
    int arg;

    if (argc < 2)
        arg = 0;
    else
        arg = atoi(argv[1]);

    TRY {
        foo(arg);
    }
    CATCH (FOO_EXCEPTION) {
        printf("exception Foo\n");
    }
    CATCH (BAR_EXCEPTION) {
        printf("exception Bar\n");
    }
    // CATCH (BAZ_EXCEPTION) {
    //     printf("exception Baz\n");
    // }
    FINALLY {
        printf("finally\n");
    }
    ENDTRY;

    return 0;
}