# Chapter 03-08: Compute Image Filter

project(chapter0208ComputeImageFilter)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    vk_common
    Vulkan::Vulkan
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-08"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/08-compute-image-filter"
)

# Copy shaders (compute + display shaders)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/chapter02/08-compute-image-filter/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/comp.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
        ${CMAKE_BINARY_DIR}/chapter02/08-compute-image-filter/shaders/
    COMMENT "Copying shaders"
)
