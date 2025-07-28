# ====添加头文件====
include_directories(${CMAKE_SOURCE_DIR}/include)

# ====添加源文件====
file(GLOB_RECURSE Frame_Source_files
    ${CMAKE_SOURCE_DIR}/cpp/*.cpp
)

add_library(Graph_KIT ${Frame_Source_files})
