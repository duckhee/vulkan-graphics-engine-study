#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <iostream>

namespace vk
{
    /**
     * Queue Family 인덱스 구조체
     *
     * Vulkan에서 GPU 작업은 Queue를 통해 제출됩니다.
     * 각 Queue Family는 특정 유형의 작업을 지원합니다:
     * - Graphics: 그래픽 렌더링
     * - Compute: 컴퓨트 셰이더
     * - Transfer: 메모리 복사
     * - Present: 화면 출력
     */
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;  // 그래픽 작업용 큐
        std::optional<uint32_t> presentFamily;   // 화면 출력용 큐

        bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    /**
     * VulkanDevice 모듈
     *
     * 학습 목표:
     * 1. Physical Device (GPU) 선택 방법
     * 2. Logical Device 생성 이유
     * 3. Queue Family의 개념
     *
     * Physical Device: 실제 GPU 하드웨어
     * Logical Device: 애플리케이션이 GPU와 통신하는 인터페이스
     */
    class VulkanDevice
    {
    public:
        VulkanDevice() = default;
        virtual ~VulkanDevice();

        // Delete copy
        VulkanDevice(const VulkanDevice&) = delete;
        VulkanDevice& operator=(const VulkanDevice&) = delete;

        /**
         * Physical Device (GPU) 선택
         * 적합한 GPU를 찾아 선택합니다.
         */
        void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

        /**
         * Logical Device 생성
         * GPU와 통신할 수 있는 인터페이스를 만듭니다.
         */
        void createLogicalDevice(bool enableValidationLayers,
                                 const std::vector<const char*>& validationLayers);

        /**
         * 정리
         */
        void destroy();

        // Getters
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
        VkDevice getDevice() const { return device; }
        VkQueue getGraphicsQueue() const { return graphicsQueue; }
        VkQueue getPresentQueue() const { return presentQueue; }
        QueueFamilyIndices getQueueFamilyIndices() const { return queueIndices; }
        const std::vector<const char*>& getDeviceExtensions() const { return deviceExtensions; }

        /**
         * Queue Family 인덱스 찾기
         */
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    protected:
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;  // Reference for queue family lookup

        QueueFamilyIndices queueIndices;

        // Device extensions (Swapchain은 필수)
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef __APPLE__
            "VK_KHR_portability_subset"
#endif
        };

    private:
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    };
}
