#include <stdio.h>
#include "baselib.h"
#include "complicated.h"

void complicated_foo(void)
{
    printf("complicated_foo\n");
}

int complicated_bar(int a, int b)
{
    return baselib_foo(a, b);
}

#ifdef ENABLE_XXX
void complicated_xxx(int v)
{
    printf("XXX: %d\n", v);
}
#endif