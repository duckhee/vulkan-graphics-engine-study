#include "vk_swapchain.h"
#include <stdexcept>
#include <limits>
#include <algorithm>

namespace vk
{
    VulkanSwapchain::~VulkanSwapchain()
    {
        destroy();
    }

    void VulkanSwapchain::create(VkDevice dev,
                                  VkPhysicalDevice physicalDevice,
                                  VkSurfaceKHR surface,
                                  uint32_t graphicsFamily,
                                  uint32_t presentFamily,
                                  uint32_t width,
                                  uint32_t height,
                                  int framesInFlight)
    {
        device = dev;

        // Swapchain 지원 정보 조회
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

        // 최적의 설정 선택
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

        // 이미지 개수 결정 (Double Buffering = 2, Triple = 3)
        uint32_t imageCount = static_cast<uint32_t>(framesInFlight);

        // min/max 범위 내로 조정
        if (imageCount < swapChainSupport.capabilities.minImageCount)
        {
            imageCount = swapChainSupport.capabilities.minImageCount;
        }
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        // Swapchain 생성 정보
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;  // 일반 렌더링은 1 (스테레오 3D는 2)
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  // 렌더 타겟으로 사용

        // Queue Family 설정
        uint32_t queueFamilyIndices[] = {graphicsFamily, presentFamily};

        if (graphicsFamily != presentFamily)
        {
            // 다른 Queue Family: 이미지 공유 모드 사용
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            // 같은 Queue Family: 독점 모드 (성능 좋음)
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;  // 회전 없음
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;  // 알파 블렌딩 없음
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;  // 가려진 픽셀 무시 (성능 향상)
        createInfo.oldSwapchain = VK_NULL_HANDLE;  // 재생성 시 이전 swapchain

        // Swapchain 생성
        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create swap chain!");
        }

        // Swapchain 이미지 핸들 가져오기
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;

        std::cout << "✓ Swapchain created (" << imageCount << " images, "
                  << extent.width << "x" << extent.height << ")\n";
    }

    void VulkanSwapchain::createImageViews()
    {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++)
        {
            // Image View: 이미지를 어떻게 해석할지 정의
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;  // 2D 텍스처로 해석
            createInfo.format = swapChainImageFormat;

            // 색상 채널 매핑 (기본값 유지)
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            // 이미지의 어떤 부분을 사용할지
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create image views!");
            }
        }

        std::cout << "✓ Image views created (" << swapChainImageViews.size() << " views)\n";
    }

    void VulkanSwapchain::destroy()
    {
        if (device != VK_NULL_HANDLE)
        {
            // Image Views 정리
            for (auto imageView : swapChainImageViews)
            {
                if (imageView != VK_NULL_HANDLE)
                    vkDestroyImageView(device, imageView, nullptr);
            }
            swapChainImageViews.clear();

            // Swapchain 정리
            if (swapChain != VK_NULL_HANDLE)
            {
                vkDestroySwapchainKHR(device, swapChain, nullptr);
                swapChain = VK_NULL_HANDLE;
            }
        }
        // Note: swapChainImages는 swapchain 소멸 시 자동 정리
    }

    SwapChainSupportDetails VulkanSwapchain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;

        // Surface 기능 조회
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        // 지원 포맷 조회
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        // Present Mode 조회
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR VulkanSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        // 선호: sRGB 색 공간의 BGRA 32비트
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        // 없으면 첫 번째 사용
        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        // Present Mode:
        // - IMMEDIATE: 즉시 출력 (티어링 발생 가능)
        // - FIFO: V-Sync (항상 지원)
        // - FIFO_RELAXED: V-Sync (늦으면 즉시 출력)
        // - MAILBOX: Triple Buffering (가장 최신 이미지 사용)

        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;  // 가장 선호
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;  // 기본값 (항상 지원)
    }

    VkExtent2D VulkanSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                                                  uint32_t width, uint32_t height)
    {
        // 일부 윈도우 시스템은 특별한 값을 사용
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            // 수동으로 설정
            VkExtent2D actualExtent = { width, height };

            actualExtent.width = std::clamp(actualExtent.width,
                capabilities.minImageExtent.width,
                capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height,
                capabilities.minImageExtent.height,
                capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }
}
