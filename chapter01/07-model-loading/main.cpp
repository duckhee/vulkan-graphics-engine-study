#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>

// TODO: Implement Chapter 07 - Model Loading
// - Add tinyobjloader to vcpkg.json
// - Load .obj file and parse vertices/indices
// - Create large vertex/index buffers
// - Render complex mesh

class ModelLoadingApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("07: Model Loading");
        ImGui::Text("TODO: Implement OBJ model loading");
        ImGui::Text("Goal: Load and render Stanford Bunny");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-07: Model Loading\n";
    std::cout << "=============================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Model Loading");
        ModelLoadingApp app;
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
