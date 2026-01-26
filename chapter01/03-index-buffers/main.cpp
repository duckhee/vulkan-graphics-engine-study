#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>

// TODO: Implement Chapter 03 - Index Buffers
// - Reuse vertex buffer from chapter02
// - Create index buffer
// - Use vkCmdDrawIndexed instead of vkCmdDraw

class IndexBufferApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("03: Index Buffers");
        ImGui::Text("TODO: Implement index buffer example");
        ImGui::Text("Goal: Reduce vertex duplication with indices");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-03: Index Buffers\n";
    std::cout << "=============================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Index Buffers");
        IndexBufferApp app;
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
