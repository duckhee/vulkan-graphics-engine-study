# Chapter 00-03: Input

add_executable(ch00-03
    main.cpp
)

target_link_libraries(ch00-03 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-03 PROPERTIES
    OUTPUT_NAME "ch00-03"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/03-input"
)