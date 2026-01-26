# Chapter 01-10: Ray Tracing Triangle

project(chapter0110RayTracingTriangle)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch01-10"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter01/10-raytracing-triangle"
)

# Copy shaders
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter01/10-raytracing-triangle/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raygen.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/miss.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/closesthit.spv
        ${CMAKE_BINARY_DIR}/chapter01/10-raytracing-triangle/shaders/
    COMMENT "Copying ray tracing shaders to chapter01/10-raytracing-triangle"
)
