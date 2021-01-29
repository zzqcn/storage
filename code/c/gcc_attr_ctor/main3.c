#include <stdio.h>
#include <dlfcn.h>
#include "foolib.h"

typedef void (*func_t) (void);

int main(void)
{
    void* h;
    func_t f;

#if 1
    h = dlopen("./libfoo.so", RTLD_LAZY);
#else
    h = dlopen("./libfoo.so", RTLD_NOW);
#endif
    f = dlsym(h, "foo_api");
    f();
    printf("hello in main3\n");
    dlclose(h);

    return 0;
}
