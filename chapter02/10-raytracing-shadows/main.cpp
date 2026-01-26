#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>
#include <glm/glm.hpp>

class ShaderApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        ImGui::Begin("Shader Example");
        ImGui::Text("TODO: Implement");
        ImGui::End();
    }
};

int main()
{
    try {
        vk::Window window(800, 600, "Shader");
        ShaderApp app;
        app.init(window);
        while (!window.shouldClose()) {
            window.pollEvents();
            app.drawFrame();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
