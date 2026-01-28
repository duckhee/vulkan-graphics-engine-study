/**
 * Track B - Step 1: Instance & Device
 *
 * 학습 목표:
 * - VulkanInstance 모듈 사용법
 * - VulkanDevice 모듈 사용법
 * - Vulkan 초기화의 첫 단계 이해
 *
 * 이 예제에서는:
 * 1. Vulkan Instance 생성 (Validation Layers 포함)
 * 2. Physical Device (GPU) 선택
 * 3. Logical Device 생성
 *
 * 아직 화면에 아무것도 표시하지 않습니다.
 * 콘솔에 초기화 성공 메시지만 출력됩니다.
 */

#include <iostream>
#include <vk_window.h>
#include <vk_instance.h>
#include <vk_device.h>

int main()
{
    try
    {
        // 1. 윈도우 생성 (Surface 생성을 위해 필요)
        vk::Window window(800, 600, "Track B - Step 1: Instance & Device");

        // 2. Vulkan Instance 생성
        vk::VulkanInstance instance;
        instance.create("Instance & Device Demo", true);  // Validation Layer 활성화
        instance.setupDebugMessenger();

        // 3. Surface 생성 (윈도우와 Vulkan 연결)
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(instance.getInstance(), window.getHandle(),
                                    nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface!");
        }
        std::cout << "✓ Window surface created\n";

        // 4. Physical Device 선택 & Logical Device 생성
        vk::VulkanDevice device;
        device.pickPhysicalDevice(instance.getInstance(), surface);
        device.createLogicalDevice(instance.isValidationEnabled(),
                                   instance.getValidationLayers());

        // 성공!
        std::cout << "\n=== 초기화 완료! ===\n";
        std::cout << "Vulkan Instance, Device, Queues가 준비되었습니다.\n";
        std::cout << "아직 렌더링은 없습니다. (다음 단계에서 Swapchain 추가)\n";
        std::cout << "\n윈도우를 닫으면 프로그램이 종료됩니다.\n";

        // 메인 루프 (이벤트 처리만)
        while (!window.shouldClose())
        {
            window.pollEvents();
        }

        // 정리 (역순)
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
