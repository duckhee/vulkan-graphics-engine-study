# Chapter 02-01: Vertex Animation
# Push Constants를 사용한 Vertex Shader 애니메이션

project(chapter0201VertexAnimation)

add_executable(${PROJECT_NAME}
    main.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    imgui::imgui
    glm::glm
)

# Include ImGui Vulkan backend from common
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common/imgui_impl_vulkan.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-01-vertex-animation"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/01-vertex-animation"
)

# Copy shaders to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/chapter02/01-vertex-animation/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
        ${CMAKE_BINARY_DIR}/chapter02/01-vertex-animation/shaders/
    COMMENT "Copying shaders for ch02-01"
)
