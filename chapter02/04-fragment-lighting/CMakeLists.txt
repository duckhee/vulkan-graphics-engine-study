# Chapter 02-04: Fragment Lighting (UBO + Phong)
# Uniform Buffer Object를 사용한 Phong/Blinn-Phong 라이팅

project(chapter0204FragmentLighting)

add_executable(${PROJECT_NAME}
    main.cpp
)

# GLFW와 Vulkan 링크
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# ImGui (vcpkg에서 설치)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    Vulkan::Vulkan
    imgui::imgui
)

# ImGui Vulkan 백엔드 (common 폴더에서)
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common/imgui_impl_vulkan.cpp
    ${CMAKE_SOURCE_DIR}/common/imgui_impl_glfw.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-04-fragment-lighting"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02"
)

# Copy shaders to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv"
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/"
    COMMENT "Copying lighting shaders for ch02-04"
)
