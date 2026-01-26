# Chapter 02-02: Fragment Gradients
# Fragment Shader에서 다양한 그라데이션 효과 구현

project(chapter0202FragmentGradients)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    imgui::imgui
    glm::glm
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common/imgui_impl_vulkan.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/common
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-02-fragment-gradients"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/02-fragment-gradients"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/chapter02/02-fragment-gradients/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
        ${CMAKE_BINARY_DIR}/chapter02/02-fragment-gradients/shaders/
    COMMENT "Copying shaders for ch02-02"
)
