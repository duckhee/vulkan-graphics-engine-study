# Chapter 00-04: Layout

add_executable(ch00-04
    main.cpp
)

target_link_libraries(ch00-04 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-04 PROPERTIES
    OUTPUT_NAME "ch00-04"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/04-layout"
)