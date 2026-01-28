#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace vk
{
    /**
     * VulkanRenderPass 모듈
     *
     * 학습 목표:
     * 1. Render Pass의 역할 이해
     * 2. Attachment의 개념 (Color, Depth)
     * 3. Subpass와 Dependency
     * 4. Framebuffer의 역할
     *
     * Render Pass: 렌더링 작업의 "청사진"
     * - 어떤 attachment(출력 대상)을 사용할지
     * - 렌더링 전후로 attachment를 어떻게 처리할지
     * - subpass 간의 의존성
     *
     * Framebuffer: Render Pass의 실제 구현
     * - Render Pass + Image Views = 실제 렌더 타겟
     */
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass() = default;
        virtual ~VulkanRenderPass();

        // Delete copy
        VulkanRenderPass(const VulkanRenderPass&) = delete;
        VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;

        /**
         * Render Pass 생성
         * @param imageFormat Swapchain 이미지 포맷
         */
        void createRenderPass(VkDevice device, VkFormat imageFormat);

        /**
         * Framebuffers 생성
         * 각 Swapchain 이미지에 대해 하나씩 생성
         */
        void createFramebuffers(const std::vector<VkImageView>& imageViews, VkExtent2D extent);

        /**
         * 정리
         */
        void destroy();

        // Getters
        VkRenderPass getRenderPass() const { return renderPass; }
        const std::vector<VkFramebuffer>& getFramebuffers() const { return framebuffers; }

    protected:
        VkDevice device = VK_NULL_HANDLE;  // Reference (not owned)
        VkRenderPass renderPass = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> framebuffers;
    };
}
