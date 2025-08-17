#!/bin/bash

# 定义项目根目录
SCRIPT_DIR="$PWD/$(dirname "$0")"

# 定义构建目录
BUILD_DIR="$SCRIPT_DIR/build"

#定义bin目录
BIN_DIR="$SCRIPT_DIR/bin"




# 检查命令参数
if [ $# -gt 2 ]; then
    echo "Error: Too many arguments."
    echo "Usage: ./build.sh [build(default)|run] [v1|v2|v3(default)]"
    echo "  build: 编译项目(默认)"
    echo "  run:   运行项目"
    echo "  v1:    使用 ThreadPool 版本"
    echo "  v2:    使用 ThreadPoolV2 版本"
    echo "  v3:    使用 ThreadPoolV3 版本(默认)"
    echo ""
    echo "示例:"
    echo "  ./build.sh build v3    # 使用ThreadPoolV3编译项目"
    echo "  ./build.sh run         # 运行已编译的程序"
    echo "  ./build.sh build       # 使用默认v3版本编译"
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
        mkdir -p "$BUILD_DIR" "$BIN_DIR"
        
        # 进入构建目录并执行编译
        cd "$BUILD_DIR" || exit 1
        echo "Starting build in $BUILD_DIR..."
        # 根据参数设置CMake选项
        case "$THREAD_POOL_VERSION" in
            v1)
                CMAKE_THREAD_POOL_OPTION="-DUSE_THREAD_POOL_V1=ON"
                ;;
            v2)
                CMAKE_THREAD_POOL_OPTION="-DUSE_THREAD_POOL_V2=ON"
                ;;
            v3)
                CMAKE_THREAD_POOL_OPTION="-DUSE_THREAD_POOL_V3=ON"
                ;;
            *)
                echo "Error: Unknown thread pool version '$THREAD_POOL_VERSION'. Use v1, v2, or v3."
                exit 1
                ;;
        esac
        # 检查操作系统类型并选择合适的 CMake 生成器
        if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
            # Windows 系统使用 MinGW Makefiles
            cmake -G "MinGW Makefiles" $CMAKE_THREAD_POOL_OPTION ..
            mingw32-make -j$(nproc)  # Windows 上使用 
        else
            # 类 Unix 系统 (如 Ubuntu) 使用默认生成器
            cmake $CMAKE_THREAD_POOL_OPTION ..
            make -j$(nproc)          # 类 Unix 系统上使用 make
        fi
        # make install         # 安装到BIN_DIR（需CMakeLists.txt配置INSTALL_PREFIX）
        echo "Build completed. Executable in $BIN_DIR"
        ;;
        
    run)
    #=================================================================
    # 运行可执行文件（需根据实际项目名称修改）
    #=================================================================
        EXECUTABLE="$BIN_DIR/mytest"

          # 替换为实际的可执行文件名
        echo "Looking for executable: $EXECUTABLE"
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