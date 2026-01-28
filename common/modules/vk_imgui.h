#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace vk
{
    /**
     * VulkanImGui 모듈
     *
     * 학습 목표:
     * 1. ImGui를 Vulkan에 통합하는 방법
     * 2. Descriptor Pool의 역할
     * 3. ImGui 렌더링 흐름
     *
     * ImGui: 즉시 모드 GUI 라이브러리
     * - 디버깅, 프로파일링 UI에 적합
     * - 매 프레임 UI 상태를 다시 구성
     */
    class VulkanImGui
    {
    public:
        VulkanImGui() = default;
        virtual ~VulkanImGui();

        // Delete copy
        VulkanImGui(const VulkanImGui&) = delete;
        VulkanImGui& operator=(const VulkanImGui&) = delete;

        /**
         * ImGui 초기화
         */
        void init(VkInstance instance,
                  VkPhysicalDevice physicalDevice,
                  VkDevice device,
                  uint32_t queueFamily,
                  VkQueue queue,
                  VkRenderPass renderPass,
                  uint32_t imageCount,
                  GLFWwindow* window);

        /**
         * 새 프레임 시작 (매 프레임 호출)
         */
        void newFrame();

        /**
         * ImGui 렌더링 명령 기록
         * @param commandBuffer 명령을 기록할 Command Buffer
         */
        void render(VkCommandBuffer commandBuffer);

        /**
         * 정리
         */
        void destroy();

        bool isInitialized() const { return initialized; }

    protected:
        VkDevice device = VK_NULL_HANDLE;  // Reference (not owned)
        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
        bool initialized = false;

    private:
        void createDescriptorPool();
    };
}
