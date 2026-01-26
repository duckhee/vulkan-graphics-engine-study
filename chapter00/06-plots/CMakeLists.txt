# Chapter 00-06: Plots

add_executable(ch00-06
    main.cpp
)

target_link_libraries(ch00-06 PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output directory
set_target_properties(ch00-06 PROPERTIES
    OUTPUT_NAME "ch00-06"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter00/06-plots"
)
