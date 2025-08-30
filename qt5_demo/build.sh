#!/bin/bash
set -e  # 脚本出错时立即退出

# 定义构建目录
BUILD_DIR="build"

# 如果没有 build 目录就创建
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# 运行 cmake 配置
cmake ..

# 编译
make -j$(nproc)

# 运行可执行文件
./Qt5Demo
