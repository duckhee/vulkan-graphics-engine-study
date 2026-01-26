#include "vk_window.h"
#include <iostream>

namespace vk
{
    Window::Window(int width, int height, const std::string& title)
        : width(width), height(height), title(title)
    {
        initWindow();
    }

    Window::~Window()
    {
        if (window)
        {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    void Window::initWindow()
    {
        // Initialize GLFW
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // Don't create an OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Make window non-resizable for now (we'll handle resize later)
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create window
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        std::cout << "✓ GLFW window created (" << width << "x" << height << ")\n";
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(window);
    }

    void Window::pollEvents() const
    {
        glfwPollEvents();
    }
}
