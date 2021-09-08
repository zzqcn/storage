/**
 * @file
 * @author zzqcn
 * @brief 基于setjmp/longjmp实现的极简异常处理,
 * 添加了finally语句中判断是否有异常
 * @see http://groups.di.unipi.it/~nids/docs/longjump_try_trow_catch.html
 * @date 2021.09.09
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;
const char* exception_strings[] = {"none", "foo", "bar", "baz", "unknown"};

static inline const char* exception_to_string(int exp)
{
    if (exp < 0 || exp > 3)
        exp = 4;
    return exception_strings[exp];
}

// clang-format off
#define TRY do { int _exp = setjmp(env); switch (_exp) { case 0: while (1) {
#define CATCH(x) break; case x:
#define HAS_EXCEPTION _exp != 0
#define EXCEPTION_VAL (_exp)
#define EXCEPTION_STR exception_to_string(_exp)
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
        // if (HAS_EXCEPTION) {
        printf("has exception: %d(%s)\n", EXCEPTION_VAL, EXCEPTION_STR);
        // }
    }
    ENDTRY;

    return 0;
}