# Chapter 00-02: Widgets

add_executable(ch00-02
    main.cpp
)

target_link_libraries(ch00-02 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-02 PROPERTIES
    OUTPUT_NAME "ch00-02"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/02-widgets"
)