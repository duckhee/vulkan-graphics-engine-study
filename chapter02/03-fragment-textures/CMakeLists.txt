# Chapter 02-03: Fragment Textures (Procedural)
# 절차적 텍스처 패턴 생성
# ShaderExampleBase를 상속하여 보일러플레이트 코드 제거

project(chapter0203FragmentTextures)

add_executable(${PROJECT_NAME}
    main.cpp
)

# ch02_common 라이브러리 링크 (ShaderExampleBase 포함)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ch02_common
    glm::glm
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "ch02-03-fragment-textures"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/chapter02/03-fragment-textures"
)

# Copy shaders to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/chapter02/03-fragment-textures/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv
        ${CMAKE_BINARY_DIR}/chapter02/03-fragment-textures/shaders/
    COMMENT "Copying shaders for ch02-03"
)
