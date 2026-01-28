#pragma once

/**
 * ShaderExampleBase - Chapter02 예제들의 공통 베이스 클래스
 *
 * 이 클래스는 Vulkan 초기화, 메인 루프, 정리 코드를 모두 포함합니다.
 * 각 예제는 이 클래스를 상속받아 셰이더 관련 로직만 구현하면 됩니다.
 *
 * 오버라이드 가능한 함수:
 * - createGraphicsPipeline(): 각 예제의 고유한 파이프라인 생성
 * - recordCommandBuffer(): 각 예제의 드로우 콜 기록
 * - renderImGui(): 각 예제의 UI 구성
 * - onUpdate(): 매 프레임 호출되는 업데이트 로직
 */

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <optional>
#include <functional>

namespace ch02
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class ShaderExampleBase
    {
    public:
        ShaderExampleBase(const std::string& title, uint32_t width = 800, uint32_t height = 600);
        virtual ~ShaderExampleBase();

        // Delete copy
        ShaderExampleBase(const ShaderExampleBase&) = delete;
        ShaderExampleBase& operator=(const ShaderExampleBase&) = delete;

        // 메인 실행 함수
        void run();

    protected:
        // === 오버라이드 가능한 가상 함수 ===

        // 파이프라인 생성 (각 예제에서 구현)
        virtual void createGraphicsPipeline() = 0;

        // Command Buffer 기록 (각 예제에서 구현)
        virtual void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) = 0;

        // ImGui UI 렌더링 (각 예제에서 구현)
        virtual void renderImGui() = 0;

        // 매 프레임 업데이트 (선택적 오버라이드)
        virtual void onUpdate() {}

        // 추가 초기화 - 파이프라인 생성 전에 호출 (UBO, Descriptor Sets 등)
        virtual void initExtra() {}

        // 추가 정리 (선택적 오버라이드)
        virtual void cleanupExtra() {}

        // === 헬퍼 함수 (고급 예제용) ===
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                         VkMemoryPropertyFlags properties, VkBuffer& buffer,
                         VkDeviceMemory& bufferMemory);
        QueueFamilyIndices getQueueFamilyIndices() const;

        // === 헬퍼 함수 ===
        VkShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& filename);

        // === Vulkan 객체들 (파생 클래스에서 접근 가능) ===

        // Window
        GLFWwindow* window = nullptr;
        uint32_t windowWidth;
        uint32_t windowHeight;
        std::string windowTitle;

        // Vulkan core
        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        QueueFamilyIndices queueFamilyIndices;

        // Swapchain
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        // Pipeline (파생 클래스에서 설정)
        VkRenderPass renderPass = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkPipeline graphicsPipeline = VK_NULL_HANDLE;

        // Commands
        VkCommandPool commandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> commandBuffers;

        // Synchronization
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t currentFrame = 0;
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        // ImGui
        VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;

        // Extensions and validation
        const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef __APPLE__
            "VK_KHR_portability_subset"
#endif
        };

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

    private:
        // === 내부 초기화 함수들 ===
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();

        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();
        void initImGui();

        void drawFrame();

        // 헬퍼 함수
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);
    };
}
