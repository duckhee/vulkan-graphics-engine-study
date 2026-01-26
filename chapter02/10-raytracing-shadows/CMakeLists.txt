# Chapter 02-10: Ray Tracing Shadows (Software Ray Tracer)
# 소프트웨어 레이 트레이서 - Compute Shader 기반

cmake_minimum_required(VERSION 3.20)
project(chapter0210RayTracingShadows)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
    imgui::imgui
)

# Platform-specific settings
if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        VK_USE_PLATFORM_MACOS_MVK
        VK_ENABLE_BETA_EXTENSIONS
    )
endif()

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
