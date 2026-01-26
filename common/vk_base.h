#pragma once

#include <vulkan/vulkan.h>
#include "vk_window.h"
#include <vector>
#include <string>
#include <optional>
#include <iostream>

namespace vk
{
    class VulkanBase
    {
    public:
        VulkanBase();
        virtual ~VulkanBase();

        // Delete copy constructor and assignment
        VulkanBase(const VulkanBase&) = delete;
        VulkanBase& operator=(const VulkanBase&) = delete;

        void init(Window& window);
        void cleanup();
        void drawFrame();

        // ImGui
        void initImGui();
        virtual void renderImGui();

    protected:
        // Window reference
        Window* window = nullptr;

        // Vulkan objects
        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;

        // Swapchain
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;

        // Render pass and framebuffers
        VkRenderPass renderPass = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        // Graphics pipeline
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkPipeline graphicsPipeline = VK_NULL_HANDLE;

        // Command buffers
        VkCommandPool commandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> commandBuffers;

        // Synchronization
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;  // Track which frame is using each image
        uint32_t currentFrame = 0;
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        // ImGui
        VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;

        // Queue family indices
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete() const
            {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        // Device extensions
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef __APPLE__
            "VK_KHR_portability_subset"
#endif
        };

        // Validation layers
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

    private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface(Window& window);
        void pickPhysicalDevice();
        void createLogicalDevice();

        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        // Swapchain support
        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffers();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void createSyncObjects();

        // Shader helpers
        VkShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& filename);

        // ImGui helpers
        void createImGuiDescriptorPool();
        void cleanupImGui();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

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