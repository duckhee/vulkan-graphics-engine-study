#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <iostream>

namespace vk
{
    /**
     * VulkanInstance 모듈
     *
     * 학습 목표:
     * 1. Vulkan Instance가 무엇인지 이해
     * 2. Validation Layer의 역할 이해
     * 3. Debug Messenger 설정 방법
     *
     * Instance는 Vulkan 라이브러리와 애플리케이션을 연결하는 첫 단계입니다.
     * 모든 Vulkan 작업은 Instance 생성으로 시작됩니다.
     */
    class VulkanInstance
    {
    public:
        VulkanInstance() = default;
        virtual ~VulkanInstance();

        // Delete copy
        VulkanInstance(const VulkanInstance&) = delete;
        VulkanInstance& operator=(const VulkanInstance&) = delete;

        /**
         * Instance 생성
         * @param appName 애플리케이션 이름
         * @param enableValidation Validation Layer 활성화 여부
         */
        void create(const std::string& appName = "Vulkan App", bool enableValidation = true);

        /**
         * Debug Messenger 설정 (Validation Layer 메시지 수신)
         */
        void setupDebugMessenger();

        /**
         * 정리
         */
        void destroy();

        // Getters
        VkInstance getInstance() const { return instance; }
        bool isValidationEnabled() const { return enableValidationLayers; }
        const std::vector<const char*>& getValidationLayers() const { return validationLayers; }

    protected:
        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        bool enableValidationLayers = true;

        // Validation layers
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

    private:
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);
    };
}
