/**
 * Track B - Step 4: Complete Triangle
 *
 * 학습 목표:
 * - 모든 모듈을 조합하여 삼각형 렌더링
 * - VulkanPipeline, VulkanCommands, VulkanImGui 모듈 사용법
 * - 전체 Vulkan 렌더링 흐름 이해
 *
 * 이 예제에서 추가되는 것:
 * - Graphics Pipeline 생성 (Shaders 포함)
 * - Command Pool/Buffers 생성
 * - Synchronization 객체 생성
 * - 메인 렌더링 루프
 * - ImGui 통합
 *
 * 드디어 화면에 삼각형이 표시됩니다!
 */

#include <iostream>
#include <vk_window.h>
#include <vk_instance.h>
#include <vk_device.h>
#include <vk_swapchain.h>
#include <vk_renderpass.h>
#include <vk_pipeline.h>
#include <vk_commands.h>
#include <vk_imgui.h>

#include <imgui.h>
#include <imgui_impl_vulkan.h>

// Command Buffer 기록 함수
void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,
                         VkRenderPass renderPass,
                         const std::vector<VkFramebuffer>& framebuffers,
                         VkExtent2D extent,
                         VkPipeline graphicsPipeline)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    VkClearValue clearColor = {{{0.1f, 0.1f, 0.2f, 1.0f}}};  // 약간 파란 배경
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);  // 삼각형 3개 정점

    // ImGui 렌더링 (나중에 추가됨)
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

int main()
{
    try
    {
        // === 초기화 단계 ===

        // 1. 윈도우
        vk::Window window(800, 600, "Track B - Complete Triangle");

        // 2. Instance
        vk::VulkanInstance vulkanInstance;
        vulkanInstance.create("Complete Triangle", true);
        vulkanInstance.setupDebugMessenger();

        // 3. Surface
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(vulkanInstance.getInstance(), window.getHandle(),
                                    nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface!");
        }
        std::cout << "✓ Window surface created\n";

        // 4. Device
        vk::VulkanDevice vulkanDevice;
        vulkanDevice.pickPhysicalDevice(vulkanInstance.getInstance(), surface);
        vulkanDevice.createLogicalDevice(vulkanInstance.isValidationEnabled(),
                                         vulkanInstance.getValidationLayers());

        // 5. Swapchain
        vk::VulkanSwapchain swapchain;
        auto indices = vulkanDevice.getQueueFamilyIndices();
        swapchain.create(
            vulkanDevice.getDevice(),
            vulkanDevice.getPhysicalDevice(),
            surface,
            indices.graphicsFamily.value(),
            indices.presentFamily.value(),
            800, 600, 2
        );
        swapchain.createImageViews();

        // 6. Render Pass & Framebuffers
        vk::VulkanRenderPass renderPass;
        renderPass.createRenderPass(vulkanDevice.getDevice(), swapchain.getImageFormat());
        renderPass.createFramebuffers(swapchain.getImageViews(), swapchain.getExtent());

        // 7. Graphics Pipeline
        vk::VulkanPipeline pipeline;
        pipeline.createGraphicsPipeline(
            vulkanDevice.getDevice(),
            renderPass.getRenderPass(),
            swapchain.getExtent(),
            "shaders/vert.spv",
            "shaders/frag.spv"
        );

        // 8. Commands & Sync
        vk::VulkanCommands commands;
        commands.createCommandPool(vulkanDevice.getDevice(), indices.graphicsFamily.value());
        commands.createCommandBuffers(vk::VulkanCommands::MAX_FRAMES_IN_FLIGHT);
        commands.createSyncObjects(swapchain.getImageCount());

        // 9. ImGui
        vk::VulkanImGui imgui;
        imgui.init(
            vulkanInstance.getInstance(),
            vulkanDevice.getPhysicalDevice(),
            vulkanDevice.getDevice(),
            indices.graphicsFamily.value(),
            vulkanDevice.getGraphicsQueue(),
            renderPass.getRenderPass(),
            swapchain.getImageCount(),
            window.getHandle()
        );

        std::cout << "\n=== 모든 초기화 완료! ===\n";

        // === 메인 루프 ===
        uint32_t currentFrame = 0;

        while (!window.shouldClose())
        {
            window.pollEvents();

            // ImGui 새 프레임
            imgui.newFrame();

            // ImGui UI 구성
            ImGui::Begin("Track B - Complete");
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Separator();
            ImGui::Text("학습 완료!");
            ImGui::BulletText("Instance & Device");
            ImGui::BulletText("Swapchain");
            ImGui::BulletText("Render Pass");
            ImGui::BulletText("Pipeline");
            ImGui::BulletText("Commands & Sync");
            ImGui::BulletText("ImGui");
            ImGui::End();

            ImGui::Render();

            // 프레임 렌더링
            bool success = commands.drawFrame(
                swapchain.getSwapchain(),
                vulkanDevice.getGraphicsQueue(),
                vulkanDevice.getPresentQueue(),
                currentFrame,
                vk::VulkanCommands::MAX_FRAMES_IN_FLIGHT,
                [&](VkCommandBuffer cmd, uint32_t imageIndex) {
                    recordCommandBuffer(cmd, imageIndex,
                                        renderPass.getRenderPass(),
                                        renderPass.getFramebuffers(),
                                        swapchain.getExtent(),
                                        pipeline.getPipeline());
                }
            );

            if (!success)
            {
                // Swapchain 재생성 필요 (윈도우 크기 변경 등)
                // 지금은 단순히 무시
            }
        }

        // Device 대기 후 정리
        commands.waitIdle();

        // === 정리 (역순) ===
        imgui.destroy();
        commands.destroy();
        pipeline.destroy();
        renderPass.destroy();
        swapchain.destroy();
        vulkanDevice.destroy();
        vkDestroySurfaceKHR(vulkanInstance.getInstance(), surface, nullptr);
        vulkanInstance.destroy();

        std::cout << "\n✓ 정리 완료\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
