#include "vk_renderpass.h"
#include <stdexcept>

namespace vk
{
    VulkanRenderPass::~VulkanRenderPass()
    {
        destroy();
    }

    void VulkanRenderPass::createRenderPass(VkDevice dev, VkFormat imageFormat)
    {
        device = dev;

        // Attachment 설명: 렌더 타겟(출력 이미지)의 속성
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = imageFormat;                       // Swapchain과 동일
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;            // MSAA 없음
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;       // 렌더링 전: 클리어
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;     // 렌더링 후: 저장
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;   // 스텐실 미사용
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  // 이전 내용 무시
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // 화면 출력용

        // Attachment 참조: Subpass에서 이 attachment를 어떻게 사용할지
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;  // colorAttachment의 인덱스
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // 최적의 레이아웃

        // Subpass 설명: 실제 렌더링 작업 단계
        // 여러 Subpass를 사용하면 복잡한 렌더링 파이프라인 구성 가능
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;  // 그래픽 파이프라인
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // Subpass Dependency: 렌더링 전후의 동기화
        // "이미지가 사용 가능해지면 렌더링 시작"을 보장
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;  // 이전 작업 (Render Pass 외부)
        dependency.dstSubpass = 0;  // 첫 번째 subpass
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;  // 기다릴 단계
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;  // 시작할 단계
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;  // 필요한 접근 권한

        // Render Pass 생성
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create render pass!");
        }

        std::cout << "✓ Render pass created\n";
    }

    void VulkanRenderPass::createFramebuffers(const std::vector<VkImageView>& imageViews, VkExtent2D extent)
    {
        framebuffers.resize(imageViews.size());

        for (size_t i = 0; i < imageViews.size(); i++)
        {
            // Framebuffer: Render Pass의 실제 출력 대상
            // 각 Swapchain 이미지마다 하나씩 필요
            VkImageView attachments[] = {
                imageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;  // 어떤 Render Pass와 호환?
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = extent.width;
            framebufferInfo.height = extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create framebuffer!");
            }
        }

        std::cout << "✓ Framebuffers created (" << framebuffers.size() << " framebuffers)\n";
    }

    void VulkanRenderPass::destroy()
    {
        if (device != VK_NULL_HANDLE)
        {
            // Framebuffers 정리
            for (auto framebuffer : framebuffers)
            {
                if (framebuffer != VK_NULL_HANDLE)
                    vkDestroyFramebuffer(device, framebuffer, nullptr);
            }
            framebuffers.clear();

            // Render Pass 정리
            if (renderPass != VK_NULL_HANDLE)
            {
                vkDestroyRenderPass(device, renderPass, nullptr);
                renderPass = VK_NULL_HANDLE;
            }
        }
    }
}
