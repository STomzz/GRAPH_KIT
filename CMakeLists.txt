cmake_minimum_required(VERSION 3.10)
project(Graph_KIT)

# 添加选项，用于选择线程池版本
option(USE_THREAD_POOL_V1 "Use ThreadPoolV1" OFF)
option(USE_THREAD_POOL_V2 "Use ThreadPoolV2" OFF)
option(USE_THREAD_POOL_V3 "Use ThreadPoolV3" OFF) # 默认使用V3

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# ====设置可执行文件的输出目录====
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ====设置动态库的输出目录====
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# ====设置静态库的输出目录====
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# ====框架源码目录====
add_subdirectory(cpp)
# ====节点层源码====
add_subdirectory(layer)
# ====项目源码目录====
add_subdirectory(src)