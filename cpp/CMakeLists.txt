# ====添加头文件====
include_directories(${CMAKE_SOURCE_DIR}/include)

# ====添加源文件====
# 定义基础源文件列表
set(Frame_Source_files
    ${CMAKE_SOURCE_DIR}/cpp/BaseNode.cpp
    ${CMAKE_SOURCE_DIR}/cpp/Graph_stream.cpp
    ${CMAKE_SOURCE_DIR}/cpp/InputNode.cpp
    ${CMAKE_SOURCE_DIR}/cpp/Specific_Node.cpp
)

# 根据选项选择线程池版本
if(USE_THREAD_POOL_V3)
    message(STATUS "Using ThreadPoolV3")
    list(APPEND Frame_Source_files ${CMAKE_SOURCE_DIR}/cpp/ThreadPoolv3.cpp)
elseif(USE_THREAD_POOL_V2)
    message(STATUS "Using ThreadPoolV2")
    list(APPEND Frame_Source_files ${CMAKE_SOURCE_DIR}/cpp/ThreadPoolv2.cpp)
else()
    message(STATUS "Using ThreadPool")
    list(APPEND Frame_Source_files ${CMAKE_SOURCE_DIR}/cpp/ThreadPool.cpp)

    # 为ThreadPool版本定义宏
    target_compile_definitions(Graph_KIT PRIVATE USE_THREAD_POOL_V1)
endif()

add_library(Graph_KIT ${Frame_Source_files})