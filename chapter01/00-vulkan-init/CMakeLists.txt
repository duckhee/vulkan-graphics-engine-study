# Chapter 01-00: Vulkan Initialization

project(chapter0100VulkanInit)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
)

# Set output name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch01-00"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter01/00-vulkan-init"
)
