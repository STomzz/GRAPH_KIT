#!/bin/bash

# 定义项目根目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# 定义构建目录
BUILD_DIR="$SCRIPT_DIR/build"

# 定义bin目录
BIN_DIR="$SCRIPT_DIR/bin"

# 定义lib目录
LIB_DIR="$SCRIPT_DIR/lib"

# 检查命令参数
if [ $# -gt 2 ]; then
    echo "Error: Too many arguments."
    echo "Usage: ./build.sh [build(default)|run|clean] [v1|v2|v3(default)]"
    echo "  build: 编译项目(默认)"
    echo "  run:   运行项目"
    echo "  clean: 清理构建文件"
    echo "  v1:    使用 ThreadPool 版本"
    echo "  v2:    使用 ThreadPoolV2 版本"
    echo "  v3:    使用 ThreadPoolV3 版本(默认)"
    echo ""
    echo "示例:"
    echo "  ./build.sh build v3    # 使用ThreadPoolV3编译项目"
    echo "  ./build.sh run         # 运行已编译的程序"
    echo "  ./build.sh clean       # 清理构建文件"
    echo ""

    exit 1
fi

# 默认使用build命令（无参数时）
COMMAND=${1:-build}
# 默认使用v3版本
THREAD_POOL_VERSION=${2:-v3}

case $COMMAND in
    build)
        echo "项目根目录: $SCRIPT_DIR"
        echo "构建目录: $BUILD_DIR"
        echo "bin目录: $BIN_DIR"

        # 创建构建和输出目录(若不存在)
        mkdir -p "$BUILD_DIR" "$BIN_DIR" "$LIB_DIR"
        
        # 进入构建目录
        cd "$BUILD_DIR" || exit 1
        
        echo "Starting build in $BUILD_DIR..."
        
        # 根据参数设置CMake选项
        case "$THREAD_POOL_VERSION" in
            v1)
                echo "使用ThreadPoolV1版本"
                CMAKE_THREAD_POOL_OPTION="-DUSE_THREAD_POOL_V1=ON"
                ;;
            v2)
                echo "使用ThreadPoolV2版本"
                CMAKE_THREAD_POOL_OPTION="-DUSE_THREAD_POOL_V2=ON"
                ;;
            v3)
                echo "使用ThreadPoolV3版本"
                CMAKE_THREAD_POOL_OPTION="-DUSE_THREAD_POOL_V3=ON"
                ;;
            *)
                echo "Error: Unknown thread pool version '$THREAD_POOL_VERSION'. Use v1, v2, or v3."
                exit 1
                ;;
        esac
        
        #跨平台CMake配置
        if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
            # Windows 系统使用 MinGW Makefiles
            echo "配置Windows环境..."
            cmake -G "MinGW Makefiles" $CMAKE_THREAD_POOL_OPTION ..
        else
            # 类 Unix 系统使用默认生成器
            echo "配置Unix环境..."
            cmake $CMAKE_THREAD_POOL_OPTION ..
        fi
        
        # 使用跨平台兼容的构建命令 = make
        echo "开始构建..."
        cmake --build . -j 
        
        echo "Build completed. Executable in $BIN_DIR"
        ;;
        
    run)
        #=================================================================
        # 运行可执行文件
        #=================================================================
        EXECUTABLE="$BIN_DIR/mytest"
        
        echo "Looking for executable: $EXECUTABLE"
        if [ -f "$EXECUTABLE" ]; then
            echo "Running $EXECUTABLE..."
            "$EXECUTABLE"
        else
            echo "Error: Executable not found. Build first with: ./build.sh build"
            exit 1
        fi
        ;;
        
    clean)
        echo "清理构建文件..."
        rm -rf "$BUILD_DIR"/*
        rm -rf "$BIN_DIR"/*
        rm -rf "$LIB_DIR"/*
        echo "清理完成。"
        ;;
        
    clean)
        echo "debug..."
        
        ;;
    *)
        echo "Usage: ./build.sh [build|run|clean] [v1|v2|v3]"
        exit 1
        ;;
esac