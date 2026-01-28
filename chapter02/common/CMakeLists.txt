# Chapter 02 Common Library - ShaderExampleBase

add_library(ch02_common STATIC
    shader_example_base.h
    shader_example_base.cpp
)

target_include_directories(ch02_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ch02_common PUBLIC
    Vulkan::Vulkan
    glfw
    imgui::imgui
    glm::glm
)

target_compile_features(ch02_common PUBLIC cxx_std_20)
