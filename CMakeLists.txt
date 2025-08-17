cmake_minimum_required(VERSION 3.10)
project(Graph_KIT)

# 添加选项，用于选择线程池版本
option(USE_THREAD_POOL_V2 "Use ThreadPoolV2 instead of ThreadPool" ON)
option(USE_THREAD_POOL_V3 "Use ThreadPoolV3 instead of ThreadPool" OFF) # 新增

set(CMAKE_CXX_STANDARD 17)

# ====设置可执行文件的输出目录====
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ====设置动态库的输出目录====
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# ====设置静态库的输出目录====
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(cpp)
add_subdirectory(src)