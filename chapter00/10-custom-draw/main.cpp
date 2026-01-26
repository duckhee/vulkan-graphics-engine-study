#include <iostream>
#include <vk_window.h>
#include <vk_base.h>
#include <imgui.h>

class ImGuiApp : public vk::VulkanBase
{
public:
    void renderImGui() override
    {
        // TODO: Implement Example 10 - Custom Drawing
        // Example: ImDrawList, AddLine, AddRect, AddCircle

        ImGui::Begin("10: Custom Draw");
        ImGui::Text("TODO: Add custom drawing example here");
        ImGui::End();
    }
};

int main()
{
    std::cout << "Chapter 00-10: Custom Draw\n";
    std::cout << "==========================\n\n";

    try
    {
        vk::Window window(800, 600, "ImGui Tutorial - Custom Draw");
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
