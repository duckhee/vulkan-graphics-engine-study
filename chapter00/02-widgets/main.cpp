#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>

class ImGuiApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        // TODO: Implement Example 02 - Widgets
        // Example: Buttons, Checkboxes, Sliders, etc.

        ImGui::Begin("02: Widgets");
        ImGui::Text("TODO: Add widgets example here");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 00-02: Widgets\n";
    std::cout << "======================\n\n";

    try
    {
        vk::Window window(800, 600, "ImGui Tutorial - Widgets");
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
