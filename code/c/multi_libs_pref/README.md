# 通过库插件配置示例

## 演示目的

条件:
- 动态库插件实现为相同代码, 演示同一个插件的不同版本
- 通过 `dlopen()` 方式加载动态库插件
- 通过接口修改插件的配置项, 配置值在内部用静态全局变量实现

主程序通过加载同一个动态库文件或不同动态库文件, 比较其中的差异.

当加载同一个动态库文件时, `dlopen()` 返回的句柄相同, 对配置项的修改会覆盖; 当加载不同动态库文件时则不同, 不同库可以有不同的配置项, 不会互相影响.

## 编译运行

系统为 Ubuntu 20.04. 设在当前源码根目录.

编译:
```bash
$ meson <builddir>
$ cd <builddir>
$ ninja
```

运行, 加载同一个库文件:
```bash
$./pref libfoo/libfoo.so
ha: 0x55605d4c92c0, fa: 0x7f0f11b0a030
hb: 0x55605d4c92c0, fb: 0x7f0f11b0a030
fa->a: 456
fb->a: 456
```

运行, 加载不同的库文件:
```bash
$./pref libfoo/libfoo.so libfoo/libfoo2.so
ha: 0x5587274d22c0, fa: 0x7fc8d2dfd030
hb: 0x5587274d28d0, fb: 0x7fc8d2df8030
fa->a: 123
fb->a: 456
```