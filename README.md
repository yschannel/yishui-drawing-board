# Yishui Drawing Board

使用 C / C++ 和 raylib 编写的课程画板项目，包含早期练习与迭代版本。

## 目录

- `Lab04/`：较完整的画板版本，包含自由绘制、橡皮擦、直线、圆形、自定义多边形及画作保存/加载。
- `Lab01/`、`Lab02/`、`Lab03/`、`Lab02out/`、`Lab03out/`、`Lab03out2/`：开发过程中的各阶段代码；部分目录为中间版本。
- `demo/`、`domo3/` 和根目录的单文件：raylib 与图形操作练习。

建议从 `Lab04/` 开始阅读。详细快捷键见 [`Lab04/readme.txt`](Lab04/readme.txt)。

## 构建 Lab04

原项目面向 Windows，使用 MinGW-w64 / w64devkit 与 raylib 5.5。需要自行安装 raylib，并让编译器能够找到其头文件和库。本仓库不包含 raylib、编译器或预编译程序。

进入 `Lab04`，在已配置依赖的开发环境中运行 `make`。也可按实际安装位置执行：

```bash
g++ -std=c++11 -O2 draw_board.cpp PAGE.cpp FANCTION.cpp -I"RAYLIB_INCLUDE_DIR" -L"RAYLIB_LIB_DIR" -lraylib -lopengl32 -lgdi32 -lwinmm -o drawing_board.exe
```

将 `RAYLIB_INCLUDE_DIR`、`RAYLIB_LIB_DIR` 替换为自己的 raylib 目录。现有 Makefile 保留原实验写法；根目录的 CMake 文件仅对应早期单文件练习，不用于构建 Lab04。

## 使用

启动后按 `A` 进入画板，按 `P`、`E`、`L`、`O` 切换铅笔、橡皮擦、直线、圆形；`M` 输入自定义多边形。保存的 `.ydf` 文件位于运行目录的 `drawings/` 中。

原实验说明注明矩形模式存在问题，可用多边形完成。各历史版本的功能和完成度不同。

## 验证

归档时使用 Windows 上的 w64devkit GCC 15.2.0 成功编译并链接了 `Lab04`。未进行完整图形界面功能测试；早期实验版本未逐个编译。

## 归档说明

上传副本排除了实验报告、编译产物、IDE 配置、压缩包和个人画作；说明中的姓名和学号已匿名化。早期 `Homework.c` 中绑定本机字体路径的加载改为使用 raylib 默认字体。桌面原文件未修改。
