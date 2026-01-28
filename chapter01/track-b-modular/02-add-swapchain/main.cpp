/**
 * Track B - Step 2: Add Swapchain
 *
 * 학습 목표:
 * - VulkanSwapchain 모듈 사용법
 * - Swapchain의 역할 이해 (Double Buffering)
 * - Present Mode와 Surface Format 선택
 *
 * 이 예제에서 추가되는 것:
 * - Swapchain 생성
 * - Image Views 생성
 *
 * 아직 화면에 아무것도 표시하지 않습니다.
 * 다음 단계에서 Render Pass를 추가합니다.
 */

#include <iostream>
#include <vk_window.h>
#include <vk_instance.h>
#include <vk_device.h>
#include <vk_swapchain.h>

int main()
{
    try
    {
        // 1. 윈도우 생성
        vk::Window window(800, 600, "Track B - Step 2: Add Swapchain");

        // 2. Instance 생성
        vk::VulkanInstance instance;
        instance.create("Swapchain Demo", true);
        instance.setupDebugMessenger();

        // 3. Surface 생성
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(instance.getInstance(), window.getHandle(),
                                    nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface!");
        }
        std::cout << "✓ Window surface created\n";

        // 4. Device 생성
        vk::VulkanDevice device;
        device.pickPhysicalDevice(instance.getInstance(), surface);
        device.createLogicalDevice(instance.isValidationEnabled(),
                                   instance.getValidationLayers());

        // 5. Swapchain 생성 (새로 추가!)
        vk::VulkanSwapchain swapchain;
        auto indices = device.getQueueFamilyIndices();
        swapchain.create(
            device.getDevice(),
            device.getPhysicalDevice(),
            surface,
            indices.graphicsFamily.value(),
            indices.presentFamily.value(),
            800, 600,  // 윈도우 크기
            2          // Double buffering
        );

        // 6. Image Views 생성 (새로 추가!)
        swapchain.createImageViews();

        // 성공!
        std::cout << "\n=== 초기화 완료! ===\n";
        std::cout << "Swapchain: " << swapchain.getImageCount() << "개 이미지\n";
        std::cout << "크기: " << swapchain.getExtent().width << "x"
                  << swapchain.getExtent().height << "\n";
        std::cout << "\n아직 렌더링은 없습니다. (다음 단계에서 Render Pass 추가)\n";
        std::cout << "\n윈도우를 닫으면 프로그램이 종료됩니다.\n";

        // 메인 루프
        while (!window.shouldClose())
        {
            window.pollEvents();
        }

        // 정리 (역순)
        swapchain.destroy();
        device.destroy();
        vkDestroySurfaceKHR(instance.getInstance(), surface, nullptr);
        instance.destroy();

        std::cout << "\n✓ 정리 완료\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
