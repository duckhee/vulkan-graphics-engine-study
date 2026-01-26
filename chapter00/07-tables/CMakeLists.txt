# Chapter 00-07: Tables

add_executable(ch00-07
    main.cpp
)

target_link_libraries(ch00-07 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-07 PROPERTIES
    OUTPUT_NAME "ch00-07"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/07-tables"
)
