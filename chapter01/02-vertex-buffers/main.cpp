#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>

// TODO: Implement Chapter 02 - Vertex Buffers
// - Define Vertex struct with position and color
// - Create staging buffer and device local buffer
// - Copy vertex data to GPU
// - Update pipeline with vertex input state
// - Bind vertex buffer before draw

class VertexBufferApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("02: Vertex Buffers");
        ImGui::Text("TODO: Implement vertex buffer example");
        ImGui::Text("Goal: Render square using GPU vertex buffer");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 01-02: Vertex Buffers\n";
    std::cout << "==============================\n\n";

    try
    {
        vk::Window window(800, 600, "Vulkan - Vertex Buffers");
        VertexBufferApp app;
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
