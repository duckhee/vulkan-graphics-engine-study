#include "vk_device.h"
#include <stdexcept>
#include <set>
#include <string>

namespace vk
{
    VulkanDevice::~VulkanDevice()
    {
        destroy();
    }

    void VulkanDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surf)
    {
        surface = surf;

        // 시스템의 모든 GPU 열거
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        // 적합한 GPU 찾기
        for (const auto& device : devices)
        {
            if (isDeviceSuitable(device))
            {
                physicalDevice = device;
                queueIndices = findQueueFamilies(physicalDevice, surface);
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }

        // 선택된 GPU 정보 출력
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        std::cout << "✓ Physical device selected: " << deviceProperties.deviceName << "\n";
    }

    void VulkanDevice::createLogicalDevice(bool enableValidationLayers,
                                           const std::vector<const char*>& validationLayers)
    {
        // Queue 생성 정보 설정
        // Graphics와 Present가 같은 family일 수 있으므로 중복 제거
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            queueIndices.graphicsFamily.value(),
            queueIndices.presentFamily.value()
        };

        float queuePriority = 1.0f;  // 0.0 ~ 1.0 사이, 높을수록 우선
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // 사용할 GPU 기능 설정 (지금은 기본값)
        VkPhysicalDeviceFeatures deviceFeatures{};

        // Logical Device 생성 정보
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        // Device 확장 (Swapchain 필수)
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // Validation layers (호환성을 위해 설정, 최신 Vulkan에서는 무시됨)
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        // Logical Device 생성
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create logical device!");
        }

        // Queue 핸들 가져오기
        vkGetDeviceQueue(device, queueIndices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, queueIndices.presentFamily.value(), 0, &presentQueue);

        std::cout << "✓ Logical device created\n";
        std::cout << "✓ Graphics queue obtained\n";
        std::cout << "✓ Present queue obtained\n";
    }

    void VulkanDevice::destroy()
    {
        if (device != VK_NULL_HANDLE)
        {
            vkDestroyDevice(device, nullptr);
            device = VK_NULL_HANDLE;
        }
        // Note: VkPhysicalDevice는 destroy하지 않음 (Instance 소멸 시 자동 정리)
    }

    QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        // Queue Family 속성 가져오기
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        // 필요한 기능을 지원하는 Queue Family 찾기
        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            // Graphics 지원 확인
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }

            // Present 지원 확인 (Surface에 출력 가능한지)
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport)
            {
                indices.presentFamily = i;
            }

            // 둘 다 찾으면 종료
            if (indices.isComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }

    bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
    {
        // Queue Family 지원 확인
        QueueFamilyIndices indices = findQueueFamilies(device, surface);

        // 필수 확장 지원 확인
        bool extensionsSupported = checkDeviceExtensionSupport(device);

        // Swapchain 지원 확인
        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            // Swapchain 지원 세부사항은 나중에 VulkanSwapchain 모듈에서 확인
            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            swapChainAdequate = formatCount > 0 && presentModeCount > 0;
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        // 지원되는 확장 목록 가져오기
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        // 필요한 확장이 모두 있는지 확인
        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
}
