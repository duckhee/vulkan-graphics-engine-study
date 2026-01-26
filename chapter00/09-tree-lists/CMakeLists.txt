# Chapter 00-09: Tree Lists

add_executable(ch00-09
    main.cpp
)

target_link_libraries(ch00-09 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-09 PROPERTIES
    OUTPUT_NAME "ch00-09"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/09-tree-lists"
)
