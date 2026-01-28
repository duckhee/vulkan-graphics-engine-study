# Chapter 02-10: Ray Tracing Shadows (Software Ray Tracer)
# 소프트웨어 레이 트레이서 - Compute Shader 기반

project(chapter0210RayTracingShadows)

add_executable(${PROJECT_NAME}
    main.cpp
)

# vk_common 링크 (imgui_impl_vulkan 포함)
target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
    glm::glm
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-10"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/10-raytracing-shadows"
)

# Copy shaders (compute ray tracer + fullscreen display)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter02/10-raytracing-shadows/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raytrace_comp.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fullscreen_vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fullscreen_frag.spv
        ${CMAKE_BINARY_DIR}/chapter02/10-raytracing-shadows/shaders/
    COMMENT "Copying ray tracing shaders"
)
