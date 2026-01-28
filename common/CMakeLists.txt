# Common Vulkan utilities library

# 모듈 라이브러리 (개별 기능을 분리하여 학습 가능)
add_library(vk_modules STATIC
    # Window (공통)
    vk_window.h
    vk_window.cpp
    # Vulkan 모듈들
    modules/vk_instance.h
    modules/vk_instance.cpp
    modules/vk_device.h
    modules/vk_device.cpp
    modules/vk_swapchain.h
    modules/vk_swapchain.cpp
    modules/vk_renderpass.h
    modules/vk_renderpass.cpp
    modules/vk_pipeline.h
    modules/vk_pipeline.cpp
    modules/vk_commands.h
    modules/vk_commands.cpp
    modules/vk_imgui.h
    modules/vk_imgui.cpp
    # ImGui Vulkan 백엔드
    imgui_impl_vulkan.h
    imgui_impl_vulkan.cpp
)

target_include_directories(vk_modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
)

target_link_libraries(vk_modules PUBLIC
    Vulkan::Vulkan
    glfw
    imgui::imgui
    glm::glm
)

target_compile_features(vk_modules PUBLIC cxx_std_20)

# 기존 통합 라이브러리 (편의 클래스)
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