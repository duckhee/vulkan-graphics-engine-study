# Chapter 00-05: Colors

add_executable(ch00-05
    main.cpp
)

target_link_libraries(ch00-05 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-05 PROPERTIES
    OUTPUT_NAME "ch00-05"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/05-colors"
)
