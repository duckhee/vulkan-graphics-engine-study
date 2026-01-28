# Track B - Step 1: Instance & Device

add_executable(ch01-b01-instance-device main.cpp)

target_link_libraries(ch01-b01-instance-device PRIVATE
    vk_modules
)

set_target_properties(ch01-b01-instance-device PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
