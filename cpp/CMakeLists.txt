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
    message(STATUS "[CMAKE]--->Using ThreadPoolV3")

    list(APPEND Frame_Source_files ${CMAKE_SOURCE_DIR}/cpp/ThreadPoolv3.cpp)
elseif(USE_THREAD_POOL_V2)
    message(STATUS "[CMAKE]--->Using ThreadPoolV2")
    list(APPEND Frame_Source_files ${CMAKE_SOURCE_DIR}/cpp/ThreadPoolv2.cpp)
elseif(USE_THREAD_POOL_V1)
    message(STATUS "[CMAKE]--->Using ThreadPoolV1")
    list(APPEND Frame_Source_files ${CMAKE_SOURCE_DIR}/cpp/ThreadPoolV1.cpp)
else()
    message(STATUS "[CMAKE]Using ThreadPoolV3 (default)")
    list(APPEND Frame_Source_files ${CMAKE_SOURCE_DIR}/cpp/ThreadPoolv3.cpp)
endif()

# 创建库
add_library(Graph_KIT ${Frame_Source_files})

# 设置编译定义
if(USE_THREAD_POOL_V3)
    target_compile_definitions(Graph_KIT PRIVATE USE_THREAD_POOL_V3)
elseif(USE_THREAD_POOL_V2)
    target_compile_definitions(Graph_KIT PRIVATE USE_THREAD_POOL_V2)
elseif(USE_THREAD_POOL_V1)
    target_compile_definitions(Graph_KIT PRIVATE USE_THREAD_POOL_V1)
else()
    target_compile_definitions(Graph_KIT PRIVATE USE_THREAD_POOL_V3)
endif()