#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO: Implement Chapter 04 - Uniform Buffers & MVP Transform
// - Create descriptor set layout for UBO
// - Create uniform buffers (per frame)
// - Update model matrix each frame (rotation)
// - Bind descriptor sets in command buffer

class UniformBufferApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("04: Uniform Buffers");
        ImGui::Text("TODO: Implement uniform buffer and MVP transform");
        ImGui::Text("Goal: Rotating square with MVP matrices");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-04: Uniform Buffers\n";
    std::cout << "===============================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Uniform Buffers");
        UniformBufferApp app;
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
