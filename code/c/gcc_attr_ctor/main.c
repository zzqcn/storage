#include <stdio.h>

__attribute__((constructor(101))) static void func(void)
{
    printf("called early in main - 101\n");
}

__attribute__((constructor(110))) static void func2(void)
{
    printf("called early in main - 110\n");
}

int main(void)
{
    printf("hello in main\n");
    return 0;
}