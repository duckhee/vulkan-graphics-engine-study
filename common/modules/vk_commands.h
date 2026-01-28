#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <functional>

namespace vk
{
    /**
     * VulkanCommands 모듈
     *
     * 학습 목표:
     * 1. Command Pool의 역할
     * 2. Command Buffer 사용법
     * 3. Synchronization (Semaphore, Fence)
     * 4. Double/Triple Buffering 구현
     *
     * Command Buffer: GPU에 보낼 명령을 기록
     * - 미리 명령을 기록해두고, 매 프레임 제출
     *
     * Synchronization:
     * - Semaphore: GPU 작업 간 동기화 (이미지 획득 <-> 렌더링 완료)
     * - Fence: CPU-GPU 동기화 (CPU가 GPU 완료를 기다림)
     */
    class VulkanCommands
    {
    public:
        VulkanCommands() = default;
        virtual ~VulkanCommands();

        // Delete copy
        VulkanCommands(const VulkanCommands&) = delete;
        VulkanCommands& operator=(const VulkanCommands&) = delete;

        /**
         * Command Pool 생성
         */
        void createCommandPool(VkDevice device, uint32_t queueFamilyIndex);

        /**
         * Command Buffers 할당
         * @param count 할당할 개수 (보통 MAX_FRAMES_IN_FLIGHT)
         */
        void createCommandBuffers(uint32_t count);

        /**
         * Synchronization 객체 생성
         * @param imageCount Swapchain 이미지 개수
         */
        void createSyncObjects(uint32_t imageCount);

        /**
         * 정리
         */
        void destroy();

        // Getters
        VkCommandPool getCommandPool() const { return commandPool; }
        const std::vector<VkCommandBuffer>& getCommandBuffers() const { return commandBuffers; }
        VkCommandBuffer getCommandBuffer(uint32_t index) const { return commandBuffers[index]; }

        // Sync objects
        VkSemaphore getImageAvailableSemaphore(uint32_t frame) const { return imageAvailableSemaphores[frame]; }
        VkSemaphore getRenderFinishedSemaphore(uint32_t frame) const { return renderFinishedSemaphores[frame]; }
        VkFence getInFlightFence(uint32_t frame) const { return inFlightFences[frame]; }

        /**
         * 프레임 렌더링
         * @param recordCallback Command Buffer 기록 콜백
         * @return true = 성공, false = Swapchain 재생성 필요
         */
        bool drawFrame(VkSwapchainKHR swapchain,
                       VkQueue graphicsQueue,
                       VkQueue presentQueue,
                       uint32_t& currentFrame,
                       uint32_t maxFramesInFlight,
                       std::function<void(VkCommandBuffer, uint32_t)> recordCallback);

        /**
         * Device가 유휴 상태가 될 때까지 대기
         */
        void waitIdle();

        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    protected:
        VkDevice device = VK_NULL_HANDLE;  // Reference (not owned)
        VkCommandPool commandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> commandBuffers;

        // Synchronization
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
    };
}
