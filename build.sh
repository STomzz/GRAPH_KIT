#!/bin/bash

# 定义项目根目录
SCRIPT_DIR="$PWD/$(dirname "$0")"

# 定义构建目录
BUILD_DIR="$SCRIPT_DIR/build"

#定义bin目录
BIN_DIR="$SCRIPT_DIR/bin"

echo "项目根目录: $SCRIPT_DIR"
echo "构建目录: $BUILD_DIR"
echo "bin目录: $BIN_DIR"

# 检查命令参数
if [ $# -gt 1 ]; then
    echo "Usage: ./build.sh [build|run]"
    exit 1
fi



# 默认使用build命令（无参数时）
COMMAND=${1:-build}

case $COMMAND in
    build)
        # 创建构建和输出目录(若不存在)
        mkdir -p "$BUILD_DIR" "$BIN_DIR"
        
        # 进入构建目录并执行编译
        cd "$BUILD_DIR" || exit 1
        echo "Starting build in $BUILD_DIR..."
        cmake ..  # 假设CMakeLists.txt在项目根目录
        make -j$(nproc)      # 多线程编译
        # make install         # 安装到BIN_DIR（需CMakeLists.txt配置INSTALL_PREFIX）
        echo "Build completed. Executable in $BIN_DIR"
        ;;
        
    run)
        # 假设可执行文件名为Graph_KIT（根据实际项目名称修改）
        EXECUTABLE="$BIN_DIR/mytest.exe"
        
        if [ -f "$EXECUTABLE" ]; then
            echo "Running $EXECUTABLE..."
            "$EXECUTABLE"
        else
            echo "Error: Executable not found. Build first with: ./build.sh build"
            exit 1
        fi
        ;;
        
    *)
        echo "Usage: ./build.sh [build|run]"
        exit 1
        ;;
esac