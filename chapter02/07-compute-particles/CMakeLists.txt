# Chapter 02-07: Compute Particles
# Compute Pipeline을 사용한 GPU 파티클 시뮬레이션

cmake_minimum_required(VERSION 3.20)
project(chapter0207ComputeParticles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# ImGui backend sources
set(IMGUI_BACKEND_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../common")

add_executable(${PROJECT_NAME}
    main.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_vulkan.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${IMGUI_BACKEND_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
    imgui::imgui
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-07-compute-particles"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy shaders
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/particle_comp.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/particle_vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/particle_frag.spv
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/
    COMMENT "Copying compute particle shaders"
)
