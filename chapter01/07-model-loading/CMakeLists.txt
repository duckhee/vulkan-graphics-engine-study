# Chapter 01-07: Model Loading

project(chapter0107ModelLoading)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch01-07"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter01/07-model-loading"
)

# Copy shaders
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter01/07-model-loading/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
        ${CMAKE_BINARY_DIR}/chapter01/07-model-loading/shaders/
    COMMENT "Copying shaders to chapter01/07-model-loading"
)
