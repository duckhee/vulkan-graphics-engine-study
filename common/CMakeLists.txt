# Common Vulkan utilities library

add_library(vk_common STATIC
    vk_window.h
    vk_window.cpp
    vk_base.h
    vk_base.cpp
    imgui_impl_vulkan.h
    imgui_impl_vulkan.cpp
)

target_include_directories(vk_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(vk_common PUBLIC
    Vulkan::Vulkan
    glfw
    imgui::imgui
    glm::glm
)

# C++20 requirement
target_compile_features(vk_common PUBLIC cxx_std_20)