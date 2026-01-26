#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>

// TODO: Implement Chapter 09 - Ray Tracing Pipeline Setup
// - Check RT extensions support
// - Create ray tracing pipeline
// - Create shader binding table
// - Create storage image for output
// - Render sky gradient with miss shader

class RayTracingSetupApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("09: Ray Tracing Setup");
        ImGui::Text("TODO: Implement ray tracing pipeline");
        ImGui::Text("Goal: Render sky gradient using ray tracing");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-09: Ray Tracing Setup\n";
    std::cout << "=================================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Ray Tracing Setup");
        RayTracingSetupApp app;
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
