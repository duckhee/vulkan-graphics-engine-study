# Chapter 00-08: Menus

add_executable(ch00-08
    main.cpp
)

target_link_libraries(ch00-08 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-08 PROPERTIES
    OUTPUT_NAME "ch00-08"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/08-menus"
)
