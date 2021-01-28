#include <stdio.h>

__attribute__((constructor)) static void func(void)
{
    printf("called early in foolib\n");
}

void foo_api(void)
{
    printf("hello in foolib\n");
}