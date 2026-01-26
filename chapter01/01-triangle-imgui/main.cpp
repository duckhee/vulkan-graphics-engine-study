#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <filesystem>

int main()
{
    std::cout << "Chapter 01: Triangle with ImGui\n";
    std::cout << "=================================\n\n";

    // DEBUG: Print current working directory
    std::cout << "Current working directory: " << std::filesystem::current_path() << "\n\n";

    try
    {
        // Step 2-A: Create GLFW window
        vk::Window window(800, 600, "Vulkan Triangle");

        // Step 2-B: Initialize Vulkan
        vk::VulkanBase vulkanApp;
        vulkanApp.init(window);

        std::cout << "\nPress ESC or close window to exit\n\n";

        // Main loop
        while (!window.shouldClose())
        {
            window.pollEvents();

            // Step 2-E: Render (clear color)
            vulkanApp.drawFrame();
        }

        std::cout << "\n✓ Application closed successfully\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}