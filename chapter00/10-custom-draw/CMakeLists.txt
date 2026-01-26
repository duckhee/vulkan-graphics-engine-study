# Chapter 00-10: Custom Draw

add_executable(ch00-10
    main.cpp
)

target_link_libraries(ch00-10 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-10 PROPERTIES
    OUTPUT_NAME "ch00-10"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/10-custom-draw"
)
