# Assignment 1

## 测试环境

OS: Arch Linux (5.10.6-zen1-1-zen)
g++: 10.2.0
CMake: 3.19.3

## 使用方法

进入文件夹后执行

```shell
$ cmake -S . -B build && cd build && make
```

进行编译, 编译得到的二进制可执行文件为 `build/zbuffer`.

要执行可执行文件, 需要一个**完全由三角形组成的** `obj` 格式的模型文件, 使用方
式是把模型文件的路径作为参数:

```shell
$ ./build/zbuffer model.obj
```

执行后, 程序会统计三种绘制方式所需时间并将信息输出到 `stdout`.

### 参数

直接执行 `./build/zbuffer` 而不提供参数, 程序将输出可选参数并退出.

- `-h|--help` 显示可选参数并退出.
- `-n|--normal` 使用法向着色 (默认).
- `-i|--interpolation` 使用读取的顶点颜色在每个三角形内进行插值着色.
- `-r|--resolution <WxH>` 指定输出图像的分辨率, 默认为 1920x1080.
- `-f|--field-of-view <fov>` 指定绘制时相机在 `y` 方向的视角 (度), 默认为 45.
- `-o|--output <path>` 指定保存的图像文件名, 默认为 `zbuffer.ppm`.

> Author: Blurgy <gy@blurgy.xyz>
> Date:   Jan 14 2021, 14:58 [CST]
