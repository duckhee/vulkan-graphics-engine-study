# Track B - Step 2: Add Swapchain

add_executable(ch01-b02-swapchain main.cpp)

target_link_libraries(ch01-b02-swapchain PRIVATE
    vk_modules
)

set_target_properties(ch01-b02-swapchain PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
