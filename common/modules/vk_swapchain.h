#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace vk
{
    /**
     * Swapchain 지원 세부사항
     *
     * GPU가 Surface에 대해 지원하는 기능들:
     * - capabilities: 이미지 크기, 개수 제한 등
     * - formats: 지원하는 픽셀 형식 (RGBA, sRGB 등)
     * - presentModes: 화면 갱신 방식 (V-Sync 등)
     */
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    /**
     * VulkanSwapchain 모듈
     *
     * 학습 목표:
     * 1. Swapchain의 역할 이해 (Double/Triple Buffering)
     * 2. Surface Format과 Present Mode 선택
     * 3. Image View의 역할
     *
     * Swapchain: 화면에 표시될 이미지들의 큐
     * - 한 이미지를 화면에 표시하는 동안 다른 이미지에 렌더링
     * - 완료되면 교체 (swap)
     */
    class VulkanSwapchain
    {
    public:
        VulkanSwapchain() = default;
        virtual ~VulkanSwapchain();

        // Delete copy
        VulkanSwapchain(const VulkanSwapchain&) = delete;
        VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

        /**
         * Swapchain 생성
         * @param framesInFlight 동시에 렌더링할 프레임 수 (일반적으로 2)
         */
        void create(VkDevice device,
                    VkPhysicalDevice physicalDevice,
                    VkSurfaceKHR surface,
                    uint32_t graphicsFamily,
                    uint32_t presentFamily,
                    uint32_t width,
                    uint32_t height,
                    int framesInFlight = 2);

        /**
         * Image Views 생성
         */
        void createImageViews();

        /**
         * 정리
         */
        void destroy();

        // Getters
        VkSwapchainKHR getSwapchain() const { return swapChain; }
        const std::vector<VkImage>& getImages() const { return swapChainImages; }
        const std::vector<VkImageView>& getImageViews() const { return swapChainImageViews; }
        VkFormat getImageFormat() const { return swapChainImageFormat; }
        VkExtent2D getExtent() const { return swapChainExtent; }
        uint32_t getImageCount() const { return static_cast<uint32_t>(swapChainImages.size()); }

        /**
         * Swapchain 지원 정보 조회
         */
        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    protected:
        VkDevice device = VK_NULL_HANDLE;  // Reference (not owned)
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;

    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);
    };
}
