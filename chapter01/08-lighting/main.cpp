#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO: Implement Chapter 08 - Blinn-Phong Lighting
// - Add normal attribute to vertex data
// - Expand UBO to include light position/color
// - Implement Blinn-Phong in fragment shader
// - ImGui controls for light position

class LightingApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("08: Lighting");
        ImGui::Text("TODO: Implement Blinn-Phong lighting");
        ImGui::Text("Goal: Realistic lighting on 3D model");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-08: Lighting\n";
    std::cout << "========================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Lighting");
        LightingApp app;
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
