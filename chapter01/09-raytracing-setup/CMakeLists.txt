# Chapter 01-09: Ray Tracing Setup

project(chapter0109RayTracingSetup)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch01-09"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter01/09-raytracing-setup"
)

# Copy shaders (ray tracing shaders)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter01/09-raytracing-setup/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raygen.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/miss.spv
        ${CMAKE_BINARY_DIR}/chapter01/09-raytracing-setup/shaders/
    COMMENT "Copying ray tracing shaders to chapter01/09-raytracing-setup"
)
