
在使用 GCC 时, 可以使用 `__attribute__((constructor))` 来注释函数, 来使得此函数在 `main()` 函数之前执行. 使用这一技巧能完成很多有趣的功能. 另外还可以提供一个优先级参数来控制多个函数的执行顺序, 优先级数字越小的越先被执行.

DPDK 用 `RTE_INIT` 封装了这一特性(rte_common.h):

```c
#define RTE_PRIORITY_LOG 101
#define RTE_PRIORITY_BUS 110
#define RTE_PRIORITY_CLASS 120
#define RTE_PRIORITY_LAST 65535

#define RTE_PRIO(prio) \
	RTE_PRIORITY_ ## prio

#ifndef RTE_INIT_PRIO /* Allow to override from EAL */
#define RTE_INIT_PRIO(func, prio) \
static void __attribute__((constructor(RTE_PRIO(prio)), used)) func(void)
#endif

#define RTE_INIT(func) \
	RTE_INIT_PRIO(func, LAST)
```

# case 1

在可执行程序中使用.

main.c:
```c
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
```

编译:
```bash
$ gcc -o main main.c
```

运行:
```bash
$ ./main
called early in main - 101
called early in main - 110
hello in main
```

# case 2

在动态库中使用, 然后链接到可执行文件, 使用隐式链接方式.

动态库代码:
```c
// foolib.h
#ifndef __FOOLIB_H__
#define __FOOLIB_H__

void foo_api(void);

#endif

// foolib.c
#include <stdio.h>

__attribute__((constructor)) static void func(void)
{
    printf("called early in foolib\n");
}

void foo_api(void)
{
    printf("hello in foolib\n");
}
```

可执行程序源码 main2.c:
```c
#include <stdio.h>
#include "foolib.h"

int main(void)
{
    foo_api();
    printf("hello in main2\n");
    return 0;
}
```

编译:
```bash
$ gcc -shared -fPIC -o libfoo.so foolib.c
$ gcc -o main2 main2.c -I. -L. -lfoo
```

运行:
```bash
$ ./main2
called early in foolib
hello in foolib
hello in main2
```

# case 3

与 case 2 代码相同, 但改变动态库的编译方式.

编译:
```bash
$ gcc -fPIC -c foolib.c
$ ld -shared -o libfoo.so foolib.o
$ gcc -o main2 main2.c -I. -L. -lfoo
```

运行:
```bash
$ ./main2
called early in foolib
hello in foolib
hello in main2
```

可以看到与 case 2 是一样的.


# case 4

在动态库中使用, 然后链接到可执行文件, 使用显式链接方式.

动态库代码不变, 但在可执行程序代码(main3.c)中使用 `dlopen` 来动态加载动态库:
```c
#include <stdio.h>
#include <dlfcn.h>
#include "foolib.h"

typedef void (*func_t) (void);

int main(void)
{
    void* h;
    func_t f;

    h = dlopen("./libfoo.so", RTLD_LAZY);
    f = dlsym(h, "foo_api");
    f();
    printf("hello in main2\n");
    dlclose(h);

    return 0;
}
```

编译:
```bash
$ gcc -shared -fPIC -o libfoo.so foolib.c
$gcc -o main3 main3.c -I. -ldl
```

运行:
```bash
$ ./main3
called early in foolib
hello in foolib
hello in main2
```

把 `dlopen` 的第 2 个参数改为 `RTLD_NOW` 执行结果也是一样的.


# case 5

与 case 4 一样, 但使用 meson 编译系统.

meson.build 文件:
```
project('attr_ctor_demo', 'c')

foolib = shared_library('foo',
    'foolib.c'
)

demo = executable('main5',
    'main3.c',
    link_with: foolib,
    link_args: '-ldl'
)
```

编译:
```bash
$ meson build
$ cd build/
$ ninja
```

运行:
```bash
$ ./main5
called early in foolib
hello in foolib
hello in main2
```

# 使用Makefile

本目录下的 Makefile 文件有多个 target, 分别对应几个 case:

| target | case |
| ------ | ---- |
| a | case 1 |
| b | case 2 |
| c | case 3 |
| d | case 4 |

每次编译新 targer 前要运行 `make clean`.


# 参考

- [GCC Manual: Declaring Attributes of Functions](https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Function-Attributes.html#Function-Attributes)
- [Are functions marked with __attribute__(constructor) run again when a shared library is reloaded?](https://stackoverflow.com/questions/39183705/are-functions-marked-with-attribute-constructor-run-again-when-a-shared-lib)
- [https://stackoverflow.com/questions/39183705/are-functions-marked-with-attribute-constructor-run-again-when-a-shared-lib](https://stackoverflow.com/questions/9472519/shared-library-constructor-not-working)
- [__attribute__((constructor)) equivalent in VC?](https://stackoverflow.com/questions/1113409/attribute-constructor-equivalent-in-vc)