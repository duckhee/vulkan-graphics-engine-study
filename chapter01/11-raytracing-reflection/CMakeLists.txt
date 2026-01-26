# Chapter 01-11: Ray Tracing Reflection

project(chapter0111RayTracingReflection)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch01-11"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter01/11-raytracing-reflection"
)

# Copy shaders
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter01/11-raytracing-reflection/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raygen.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/miss.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/closesthit.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/intersection.spv
        ${CMAKE_BINARY_DIR}/chapter01/11-raytracing-reflection/shaders/
    COMMENT "Copying ray tracing shaders to chapter01/11-raytracing-reflection"
)
