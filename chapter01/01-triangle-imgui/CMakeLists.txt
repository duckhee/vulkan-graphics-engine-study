# Chapter 01: Triangle with ImGui

project(chapter0101Triangle)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        vk_common
        Vulkan::Vulkan
)

# Set output name
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "chapter01"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter01/01-triangle-imgui"
)

# Copy shader files to build directory (same directory as executable)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter01/01-triangle-imgui/shaders
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
        ${CMAKE_BINARY_DIR}/chapter01/01-triangle-imgui/shaders/
        COMMENT "Copying shader files to chapter01/01-triangle-imgui directory"
)