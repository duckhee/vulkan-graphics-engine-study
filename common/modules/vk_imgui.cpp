#include "vk_imgui.h"
#include <stdexcept>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include "../imgui_impl_vulkan.h"

namespace vk
{
    VulkanImGui::~VulkanImGui()
    {
        destroy();
    }

    void VulkanImGui::init(VkInstance instance,
                           VkPhysicalDevice physicalDevice,
                           VkDevice dev,
                           uint32_t queueFamily,
                           VkQueue queue,
                           VkRenderPass renderPass,
                           uint32_t imageCount,
                           GLFWwindow* window)
    {
        device = dev;

        // 1. Descriptor Pool 생성 (ImGui 내부 리소스용)
        createDescriptorPool();

        // 2. ImGui 컨텍스트 생성
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // 키보드 네비게이션 활성화

        // 3. 스타일 설정
        ImGui::StyleColorsDark();

        // 4. GLFW 백엔드 초기화 (입력 처리)
        ImGui_ImplGlfw_InitForVulkan(window, true);

        // 5. Vulkan 백엔드 초기화 (렌더링)
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance;
        init_info.PhysicalDevice = physicalDevice;
        init_info.Device = device;
        init_info.QueueFamily = queueFamily;
        init_info.Queue = queue;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = descriptorPool;
        init_info.RenderPass = renderPass;
        init_info.Subpass = 0;
        init_info.MinImageCount = 2;
        init_info.ImageCount = imageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&init_info);

        // 6. 폰트 텍스처 업로드
        ImGui_ImplVulkan_CreateFontsTexture();
        vkDeviceWaitIdle(device);

        initialized = true;
        std::cout << "✓ ImGui initialized\n";
    }

    void VulkanImGui::createDescriptorPool()
    {
        // ImGui가 내부적으로 사용할 Descriptor Pool
        // 다양한 타입의 Descriptor를 넉넉하게 할당
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = std::size(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        if (vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create ImGui descriptor pool!");
        }
    }

    void VulkanImGui::newFrame()
    {
        if (!initialized) return;

        // 새 프레임 시작
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void VulkanImGui::render(VkCommandBuffer commandBuffer)
    {
        if (!initialized) return;

        // ImGui 렌더링 데이터 생성
        ImGui::Render();

        // Vulkan Command Buffer에 ImGui 렌더링 명령 기록
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    void VulkanImGui::destroy()
    {
        if (initialized)
        {
            ImGui_ImplVulkan_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            initialized = false;
        }

        if (descriptorPool != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(device, descriptorPool, nullptr);
            descriptorPool = VK_NULL_HANDLE;
        }
    }
}
