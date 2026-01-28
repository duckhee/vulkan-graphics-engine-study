# Track B - Step 4: Complete Triangle (with Pipeline, Commands, ImGui)

add_executable(ch01-b04-complete main.cpp)

target_link_libraries(ch01-b04-complete PRIVATE
    vk_modules
)

set_target_properties(ch01-b04-complete PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

# Find glslangValidator for shader compilation
find_program(GLSLANG_VALIDATOR glslangValidator HINTS $ENV{VULKAN_SDK}/bin)

if(GLSLANG_VALIDATOR)
    # Shader source files
    set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
    set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

    # Create output directory
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    # Compile vertex shader
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_DIR}/vert.spv
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER_DIR}/triangle.vert -o ${SHADER_OUTPUT_DIR}/vert.spv
        DEPENDS ${SHADER_DIR}/triangle.vert
        COMMENT "Compiling vertex shader"
    )

    # Compile fragment shader
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_DIR}/frag.spv
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER_DIR}/triangle.frag -o ${SHADER_OUTPUT_DIR}/frag.spv
        DEPENDS ${SHADER_DIR}/triangle.frag
        COMMENT "Compiling fragment shader"
    )

    # Add custom target that depends on compiled shaders
    add_custom_target(ch01-b04-shaders ALL
        DEPENDS
            ${SHADER_OUTPUT_DIR}/vert.spv
            ${SHADER_OUTPUT_DIR}/frag.spv
    )

    # Make sure shaders are compiled before the executable
    add_dependencies(ch01-b04-complete ch01-b04-shaders)
else()
    message(WARNING "glslangValidator not found - shaders will not be compiled automatically")
    # Fallback: Copy pre-compiled shaders if available
    add_custom_command(TARGET ch01-b04-complete POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_CURRENT_BINARY_DIR}/shaders"
        COMMENT "Copying shaders for Track B Step 4"
    )
endif()
