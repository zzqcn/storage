# 概述

结合《现代C++语言核心特性解析》(谢丙堃, 2021.10)一书学习 C++ 1x/2x 新特性的示例代码. 另外还参考了:

- 《学习C++20》(Paul J. Deitel等, 2023.06)
- 《高速上手C++11/14》(欧长坤, 非出版物, 电子书)
- Snort 3 源代码

以此书为准, 每一章建立一个源码, 做为代码单元(code unit). 所有代码单元都注册到全局代码单元管理器中, 在主程序(源码`cpp1x2x.cpp`)中遍历所有代码单元, 运行其代码. 有点类似于单元测试.

# 我的环境

为在不同环境中试验代码, 我准备了以下几种环境:

- Windows 11, Visual Studio 2022, Intel i5-12400, 组装物理机
- Linux (Ubuntu 20.04), GCC 9.4.0, Intel Xeon Platinum 8163, 阿里云主机
- macOS Sonoma (14.2.1), clang 15.0.0, Apple M1 Pro 芯片, MacBook Pro

所有代码都至少在这三种环境中编译运行通过.

# 编译运行

代码使用 meson 编译系统, 默认使用 ninja 做为 backend. C++ 标准设置为 C++20.

为了在各平台正确输出推导类型等, 使用了 Boost 库. 在 Windows 平台, 可设置环境变量让 meson 找到 Boost 依赖库,
方法是 `set BOOST_ROOT=<Boost库解压位置>` (临时) 或 `setx BOOST_ROOT <Boost库解压位置>` (永久). 其他平台类似, 从略.

编译运行:
```bash
$ cd <cpp1x2x root>
$ meson setup <builddir>
$ cd <builddir>
$ ninja
$ ./cpp1x2x # 或 cpp1x2x.exe (on Windows)
```

