#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO: Implement Chapter 06 - Depth Testing
// - Create depth image, memory, and image view
// - Modify render pass to add depth attachment
// - Enable depth testing in pipeline
// - Render 3D cube (36 vertices)

class DepthTestingApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("06: Depth Testing");
        ImGui::Text("TODO: Implement depth buffer and 3D cube");
        ImGui::Text("Goal: Rotating 3D cube with proper face occlusion");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-06: Depth Testing\n";
    std::cout << "=============================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Depth Testing");
        DepthTestingApp app;
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
