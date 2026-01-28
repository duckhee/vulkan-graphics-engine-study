# Track B - Step 3: Add Render Pass

add_executable(ch01-b03-renderpass main.cpp)

target_link_libraries(ch01-b03-renderpass PRIVATE
    vk_modules
)

set_target_properties(ch01-b03-renderpass PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
