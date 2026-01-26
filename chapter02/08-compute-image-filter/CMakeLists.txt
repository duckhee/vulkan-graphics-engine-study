# Chapter 02-08: Compute Image Filter
# Standalone build - vk_common 미사용

cmake_minimum_required(VERSION 3.20)
project(chapter0208ComputeImageFilter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vulkan 찾기
find_package(Vulkan REQUIRED)

# vcpkg로 설치된 패키지 찾기
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    ${CMAKE_SOURCE_DIR}/common/imgui_impl_vulkan.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
    imgui::imgui
)

# macOS 특수 처리
if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        VK_USE_PLATFORM_MACOS_MVK
        VK_ENABLE_BETA_EXTENSIONS
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-08"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Shader 파일 복사
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/filter_comp.spv"
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/fullscreen_vert.spv"
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/fullscreen_frag.spv"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/"
    COMMENT "Copying compute image filter shaders"
)
