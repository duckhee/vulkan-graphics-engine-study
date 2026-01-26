# Chapter 02-06: Geometry Explosion
# Geometry Shader를 사용한 폭발 효과

project(chapter0206GeometryExplosion)

# ImGui 소스 찾기
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    # ImGui 백엔드
    ${CMAKE_SOURCE_DIR}/common/imgui_impl_vulkan.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    imgui::imgui
    glm::glm
)

# C++17 필요
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-06"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/06-geometry-explosion"
)

# Copy shaders (geometry shader explosion)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter02/06-geometry-explosion/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/explosion_vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/explosion_geom.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/explosion_frag.spv
        ${CMAKE_BINARY_DIR}/chapter02/06-geometry-explosion/shaders/
    COMMENT "Copying explosion shaders"
)
