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

# Copy shaders from 01-triangle-imgui (VulkanBase requires vert.spv and frag.spv)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/chapter01/00-vulkan-init/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/chapter01/01-triangle-imgui/shaders/vert.spv
        ${CMAKE_SOURCE_DIR}/chapter01/01-triangle-imgui/shaders/frag.spv
        ${CMAKE_BINARY_DIR}/chapter01/00-vulkan-init/shaders/
    COMMENT "Copying shaders for ch01-00"
)
