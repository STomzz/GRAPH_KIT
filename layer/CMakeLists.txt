
# include_directories(
#     ${CMAKE_SOURCE_DIR}/layer_include
# )

# ====添加源文件====
file(GLOB_RECURSE layer_sorce
    ${CMAKE_SOURCE_DIR}/layer/*.cpp
)

add_library(layer_node ${layer_sorce})

target_include_directories(layer_node PUBLIC 
                        ${CMAKE_SOURCE_DIR}/include
                        ${CMAKE_SOURCE_DIR}/layer_include)

# target_link_libraries(layer_node Graph_KIT)
