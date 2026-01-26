# Chapter 03-10: Ray Tracing Shadows

project(chapter0210RayTracingShadows)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-10"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/10-raytracing-shadows"
)

# Copy shaders (ray tracing shaders)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter02/10-raytracing-shadows/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raygen.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/closesthit.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/miss.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadow_miss.spv
        ${CMAKE_BINARY_DIR}/chapter02/10-raytracing-shadows/shaders/
    COMMENT "Copying shaders"
)
