#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>

// Custom Vulkan app that overrides ImGui rendering
class ImGuiApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        // TODO: Implement Example 01 - Basic Window
        // Example: Simple window with text

        ImGui::Begin("01: Basic Window");
        ImGui::Text("TODO: Add example content here");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 00-01: Basic Window\n";
    std::cout << "============================\n\n";

    try
    {
        vk::Window window(800, 600, "ImGui Tutorial - Basic Window");
        ImGuiApp app;
        app.init(window);

        std::cout << "\nPress ESC or close window to exit\n\n";

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
