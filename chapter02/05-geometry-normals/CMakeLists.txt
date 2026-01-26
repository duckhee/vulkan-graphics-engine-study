# Chapter 02-05: Geometry Normals
# Geometry Shader를 사용한 노말 벡터 시각화

project(chapter0205GeometryNormals)

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
    OUTPUT_NAME "ch02-05"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/05-geometry-normals"
)

# Copy shaders (includes geometry shader and mesh shaders)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter02/05-geometry-normals/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/normals_vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/normals_geom.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/normals_frag.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/mesh_vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/mesh_frag.spv
        ${CMAKE_BINARY_DIR}/chapter02/05-geometry-normals/shaders/
    COMMENT "Copying geometry and mesh shaders"
)
