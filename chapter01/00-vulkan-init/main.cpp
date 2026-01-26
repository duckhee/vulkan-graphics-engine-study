#include <iostream>
#include <vk_window.h>
#include <vk_base.h>

// Simple Vulkan app without ImGui
class VulkanInitApp : public vk::VulkanBase
{
public:
    // Override renderImGui to skip ImGui rendering
    void renderImGui() override
    {
        // No ImGui UI for this basic example
        // This example only shows Vulkan initialization and a clear screen
    }
};

int main()
{
    std::cout << "Chapter 01-00: Vulkan Initialization\n";
    std::cout << "=====================================\n\n";

    try
    {
        // Create window
        vk::Window window(800, 600, "Vulkan Initialization");

        // Create and initialize Vulkan application
        VulkanInitApp app;
        app.init(window);

        std::cout << "\n✓ Vulkan initialization complete!\n";
        std::cout << "\nThis example demonstrates:\n";
        std::cout << "  - Vulkan instance creation\n";
        std::cout << "  - Physical device selection\n";
        std::cout << "  - Logical device creation\n";
        std::cout << "  - Swapchain setup\n";
        std::cout << "  - Render pass creation\n";
        std::cout << "  - Command buffer recording\n";
        std::cout << "\nYou should see a black window.\n";
        std::cout << "\nPress ESC or close window to exit\n\n";

        // Main loop - just clear the screen
        while (!window.shouldClose())
        {
            window.pollEvents();
            app.drawFrame();
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
