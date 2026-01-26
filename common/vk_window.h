#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

namespace vk
{
    class Window
    {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        // Delete copy constructor and assignment
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool shouldClose() const;
        void pollEvents() const;
        GLFWwindow* getHandle() const { return window; }

        int getWidth() const { return width; }
        int getHeight() const { return height; }

    private:
        GLFWwindow* window = nullptr;
        int width;
        int height;
        std::string title;

        void initWindow();
    };
}
