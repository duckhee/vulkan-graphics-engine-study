cmake_minimum_required(VERSION 3.20)
project(claude_vulkan VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages via vcpkg
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Vulkan SDK information
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK not found! Please install from https://vulkan.lunarg.com/")
endif()

message(STATUS "=== Package Information ===")
message(STATUS "Vulkan SDK Version: ${Vulkan_VERSION}")
message(STATUS "Vulkan Include Dir: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Vulkan Library: ${Vulkan_LIBRARIES}")
message(STATUS "GLFW3 found: ${glfw3_FOUND}")
message(STATUS "ImGui found: ${imgui_FOUND}")
message(STATUS "GLM found: ${glm_FOUND}")
message(STATUS "==========================")

# Build configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Platform-specific settings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# macOS specific: MoltenVK requires portability enumeration
if(APPLE)
    add_compile_definitions(VK_ENABLE_BETA_EXTENSIONS)
endif()

# Add subdirectories
add_subdirectory(common)
add_subdirectory(chapter00)
add_subdirectory(chapter01)
add_subdirectory(chapter02)
