#include "vk_commands.h"
#include <stdexcept>
#include <limits>

namespace vk
{
    VulkanCommands::~VulkanCommands()
    {
        destroy();
    }

    void VulkanCommands::createCommandPool(VkDevice dev, uint32_t queueFamilyIndex)
    {
        device = dev;

        // Command Pool: Command Buffer를 할당받는 메모리 풀
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;  // 개별 버퍼 리셋 허용
        poolInfo.queueFamilyIndex = queueFamilyIndex;  // Graphics queue에서 실행

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create command pool!");
        }

        std::cout << "✓ Command pool created\n";
    }

    void VulkanCommands::createCommandBuffers(uint32_t count)
    {
        commandBuffers.resize(count);

        // Command Buffer 할당 (생성이 아닌 할당)
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;  // 직접 제출 가능
        allocInfo.commandBufferCount = count;

        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        std::cout << "✓ Command buffers allocated (" << count << ")\n";
    }

    void VulkanCommands::createSyncObjects(uint32_t imageCount)
    {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(imageCount, VK_NULL_HANDLE);

        // Semaphore: GPU 작업 간 동기화
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        // Fence: CPU-GPU 동기화
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;  // 처음에 signaled 상태로 생성

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create synchronization objects!");
            }
        }

        std::cout << "✓ Synchronization objects created\n";
    }

    void VulkanCommands::destroy()
    {
        if (device != VK_NULL_HANDLE)
        {
            // Sync objects 정리
            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            {
                if (i < renderFinishedSemaphores.size() && renderFinishedSemaphores[i] != VK_NULL_HANDLE)
                    vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
                if (i < imageAvailableSemaphores.size() && imageAvailableSemaphores[i] != VK_NULL_HANDLE)
                    vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
                if (i < inFlightFences.size() && inFlightFences[i] != VK_NULL_HANDLE)
                    vkDestroyFence(device, inFlightFences[i], nullptr);
            }
            imageAvailableSemaphores.clear();
            renderFinishedSemaphores.clear();
            inFlightFences.clear();

            // Command Pool 정리 (Command Buffers는 자동 해제)
            if (commandPool != VK_NULL_HANDLE)
            {
                vkDestroyCommandPool(device, commandPool, nullptr);
                commandPool = VK_NULL_HANDLE;
            }
        }
    }

    bool VulkanCommands::drawFrame(VkSwapchainKHR swapchain,
                                   VkQueue graphicsQueue,
                                   VkQueue presentQueue,
                                   uint32_t& currentFrame,
                                   uint32_t maxFramesInFlight,
                                   std::function<void(VkCommandBuffer, uint32_t)> recordCallback)
    {
        // 1. 이전 프레임의 Fence 대기
        // GPU가 이 프레임의 Command Buffer 사용을 마칠 때까지 대기
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        // 2. Swapchain에서 다음 이미지 획득
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
            imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            // Swapchain 재생성 필요 (윈도우 크기 변경 등)
            return false;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        // 3. 이 이미지를 사용하는 이전 프레임이 있다면 대기
        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
        {
            vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        // 4. Fence 리셋 (작업 제출 전에)
        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        // 5. Command Buffer 기록
        vkResetCommandBuffer(commandBuffers[currentFrame], 0);
        recordCallback(commandBuffers[currentFrame], imageIndex);

        // 6. Command Buffer 제출
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        // 대기할 Semaphore: 이미지가 준비될 때까지
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

        // 신호할 Semaphore: 렌더링 완료
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }

        // 7. 화면에 출력 (Present)
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;  // 렌더링 완료 대기

        VkSwapchainKHR swapChains[] = {swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            return false;  // Swapchain 재생성 필요
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swap chain image!");
        }

        // 8. 다음 프레임으로
        currentFrame = (currentFrame + 1) % maxFramesInFlight;

        return true;
    }

    void VulkanCommands::waitIdle()
    {
        if (device != VK_NULL_HANDLE)
        {
            vkDeviceWaitIdle(device);
        }
    }
}
