#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>

// TODO: Implement Chapter 05 - Texture Mapping
// - Create texture image and sampler
// - Transition image layouts
// - Update descriptor set layout for combined image sampler
// - Add texCoord to vertex data

class TextureApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("05: Texture Mapping");
        ImGui::Text("TODO: Implement texture mapping");
        ImGui::Text("Goal: Textured square with checkerboard pattern");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-05: Texture Mapping\n";
    std::cout << "===============================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Texture Mapping");
        TextureApp app;
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
