# ====添加头文件====
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# ====添加源文件====
file(GLOB_RECURSE Source_files
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(mytest ${Source_files})

target_link_libraries(mytest Graph_KIT layer_node)
