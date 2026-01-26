# Chapter 00-01: Basic Window

add_executable(ch00-01
    main.cpp
)

target_link_libraries(ch00-01 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-01 PROPERTIES
    OUTPUT_NAME "ch00-01"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/01-basic-window"
)
