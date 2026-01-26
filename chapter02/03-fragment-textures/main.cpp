/**
 * Chapter 02-03: Fragment Textures
 * 절차적 텍스처 패턴 (Checkerboard, Circles, Stripes, Procedural Noise)
 */

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <optional>
#include <chrono>
#include <stdexcept>
#include <cstring>
#include <cmath>

struct PushConstants {
    float time;
    float textureType;
    float scale;
    float param1;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class TextureApp {
public:
    void run() { initWindow(); initVulkan(); mainLoop(); cleanup(); }

private:
    GLFWwindow* window = nullptr;
    const uint32_t WIDTH = 800, HEIGHT = 600;

    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE, presentQueue = VK_NULL_HANDLE;

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores, renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;

    PushConstants pushConstants = { 0.0f, 0.0f, 8.0f, 0.0f };
    std::chrono::high_resolution_clock::time_point startTime;

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef __APPLE__
        "VK_KHR_portability_subset"
#endif
    };
    const bool enableValidationLayers =
#ifdef NDEBUG
        false;
#else
        true;
#endif

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Ch02-03: Fragment Textures", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance(); setupDebugMessenger(); createSurface();
        pickPhysicalDevice(); createLogicalDevice(); createSwapChain();
        createImageViews(); createRenderPass(); createGraphicsPipeline();
        createFramebuffers(); createCommandPool(); createCommandBuffers();
        createSyncObjects(); initImGui();
        startTime = std::chrono::high_resolution_clock::now();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            pushConstants.time = std::chrono::duration<float>(
                std::chrono::high_resolution_clock::now() - startTime).count();
            drawFrame();
        }
        vkDeviceWaitIdle(device);
    }

    void cleanup() {
        ImGui_ImplVulkan_Shutdown(); ImGui_ImplGlfw_Shutdown(); ImGui::DestroyContext();
        vkDestroyDescriptorPool(device, imguiDescriptorPool, nullptr);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }
        vkDestroyCommandPool(device, commandPool, nullptr);
        for (auto fb : swapChainFramebuffers) vkDestroyFramebuffer(device, fb, nullptr);
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
        for (auto iv : swapChainImageViews) vkDestroyImageView(device, iv, nullptr);
        vkDestroySwapchainKHR(device, swapChain, nullptr);
        vkDestroyDevice(device, nullptr);
        if (enableValidationLayers) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func) func(instance, debugMessenger, nullptr);
        }
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window); glfwTerminate();
    }

    void createInstance() {
        VkApplicationInfo appInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
        appInfo.pApplicationName = "Texture"; appInfo.apiVersion = VK_API_VERSION_1_3;

        uint32_t glfwExtCount; const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
        std::vector<const char*> extensions(glfwExts, glfwExts + glfwExtCount);
        if (enableValidationLayers) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#ifdef __APPLE__
        extensions.push_back("VK_KHR_portability_enumeration");
        extensions.push_back("VK_KHR_get_physical_device_properties2");
#endif

        VkInstanceCreateInfo createInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = (uint32_t)extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();
#ifdef __APPLE__
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        vkCreateInstance(&createInfo, nullptr, &instance);
    }

    void setupDebugMessenger() {
        if (!enableValidationLayers) return;
        VkDebugUtilsMessengerCreateInfoEXT ci{VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        ci.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        ci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        ci.pfnUserCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT,
            const VkDebugUtilsMessengerCallbackDataEXT* d, void*) { std::cerr << d->pMessage << "\n"; return VK_FALSE; };
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func) func(instance, &ci, nullptr, &debugMessenger);
    }

    void createSurface() { glfwCreateWindowSurface(instance, window, nullptr, &surface); }

    void pickPhysicalDevice() {
        uint32_t count; vkEnumeratePhysicalDevices(instance, &count, nullptr);
        std::vector<VkPhysicalDevice> devs(count);
        vkEnumeratePhysicalDevices(instance, &count, devs.data());
        for (auto& d : devs) {
            QueueFamilyIndices indices = findQueueFamilies(d);
            if (indices.isComplete()) { physicalDevice = d; break; }
        }
    }

    void createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        std::set<uint32_t> unique = { indices.graphicsFamily.value(), indices.presentFamily.value() };
        std::vector<VkDeviceQueueCreateInfo> qcis;
        float priority = 1.0f;
        for (uint32_t qf : unique) {
            VkDeviceQueueCreateInfo qci{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
            qci.queueFamilyIndex = qf; qci.queueCount = 1; qci.pQueuePriorities = &priority;
            qcis.push_back(qci);
        }
        VkPhysicalDeviceFeatures features{};
        VkDeviceCreateInfo ci{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        ci.queueCreateInfoCount = (uint32_t)qcis.size(); ci.pQueueCreateInfos = qcis.data();
        ci.pEnabledFeatures = &features;
        ci.enabledExtensionCount = (uint32_t)deviceExtensions.size();
        ci.ppEnabledExtensionNames = deviceExtensions.data();
        vkCreateDevice(physicalDevice, &ci, nullptr, &device);
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }

    void createSwapChain() {
        SwapChainSupportDetails support = querySwapChainSupport(physicalDevice);
        VkSurfaceFormatKHR fmt = support.formats[0];
        for (auto& f : support.formats) if (f.format == VK_FORMAT_B8G8R8A8_SRGB) fmt = f;
        VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;
        VkExtent2D extent = support.capabilities.currentExtent;
        uint32_t imgCount = support.capabilities.minImageCount + 1;

        VkSwapchainCreateInfoKHR ci{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
        ci.surface = surface; ci.minImageCount = imgCount; ci.imageFormat = fmt.format;
        ci.imageColorSpace = fmt.colorSpace; ci.imageExtent = extent; ci.imageArrayLayers = 1;
        ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        ci.preTransform = support.capabilities.currentTransform;
        ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        ci.presentMode = mode; ci.clipped = VK_TRUE;
        vkCreateSwapchainKHR(device, &ci, nullptr, &swapChain);
        vkGetSwapchainImagesKHR(device, swapChain, &imgCount, nullptr);
        swapChainImages.resize(imgCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imgCount, swapChainImages.data());
        swapChainImageFormat = fmt.format; swapChainExtent = extent;
    }

    void createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo ci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            ci.image = swapChainImages[i]; ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ci.format = swapChainImageFormat;
            ci.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            vkCreateImageView(device, &ci, nullptr, &swapChainImageViews[i]);
        }
    }

    void createRenderPass() {
        VkAttachmentDescription att{}; att.format = swapChainImageFormat;
        att.samples = VK_SAMPLE_COUNT_1_BIT; att.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        att.storeOp = VK_ATTACHMENT_STORE_OP_STORE; att.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        att.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        VkAttachmentReference ref{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
        VkSubpassDescription sub{}; sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        sub.colorAttachmentCount = 1; sub.pColorAttachments = &ref;
        VkSubpassDependency dep{}; dep.srcSubpass = VK_SUBPASS_EXTERNAL;
        dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        VkRenderPassCreateInfo ci{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
        ci.attachmentCount = 1; ci.pAttachments = &att;
        ci.subpassCount = 1; ci.pSubpasses = &sub;
        ci.dependencyCount = 1; ci.pDependencies = &dep;
        vkCreateRenderPass(device, &ci, nullptr, &renderPass);
    }

    void createGraphicsPipeline() {
        auto readFile = [](const std::string& fn) {
            std::ifstream f(fn, std::ios::ate | std::ios::binary);
            size_t sz = f.tellg(); std::vector<char> buf(sz);
            f.seekg(0); f.read(buf.data(), sz); return buf;
        };
        auto createModule = [&](const std::vector<char>& code) {
            VkShaderModuleCreateInfo ci{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
            ci.codeSize = code.size(); ci.pCode = (uint32_t*)code.data();
            VkShaderModule m; vkCreateShaderModule(device, &ci, nullptr, &m); return m;
        };
        VkShaderModule vert = createModule(readFile("shaders/vert.spv"));
        VkShaderModule frag = createModule(readFile("shaders/frag.spv"));

        VkPipelineShaderStageCreateInfo stages[2]{};
        stages[0] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
        stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT; stages[0].module = vert; stages[0].pName = "main";
        stages[1] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
        stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT; stages[1].module = frag; stages[1].pName = "main";

        VkPipelineVertexInputStateCreateInfo vi{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        VkPipelineInputAssemblyStateCreateInfo ia{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkViewport vp{0, 0, (float)swapChainExtent.width, (float)swapChainExtent.height, 0, 1};
        VkRect2D sc{{0,0}, swapChainExtent};
        VkPipelineViewportStateCreateInfo vs{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        vs.viewportCount = 1; vs.pViewports = &vp; vs.scissorCount = 1; vs.pScissors = &sc;

        VkPipelineRasterizationStateCreateInfo rs{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        rs.polygonMode = VK_POLYGON_MODE_FILL; rs.lineWidth = 1; rs.cullMode = VK_CULL_MODE_NONE;
        VkPipelineMultisampleStateCreateInfo ms{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        VkPipelineColorBlendAttachmentState cba{}; cba.colorWriteMask = 0xF;
        VkPipelineColorBlendStateCreateInfo cb{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
        cb.attachmentCount = 1; cb.pAttachments = &cba;

        VkPushConstantRange pcr{VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstants)};
        VkPipelineLayoutCreateInfo pli{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        pli.pushConstantRangeCount = 1; pli.pPushConstantRanges = &pcr;
        vkCreatePipelineLayout(device, &pli, nullptr, &pipelineLayout);

        VkGraphicsPipelineCreateInfo pi{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
        pi.stageCount = 2; pi.pStages = stages;
        pi.pVertexInputState = &vi; pi.pInputAssemblyState = &ia;
        pi.pViewportState = &vs; pi.pRasterizationState = &rs;
        pi.pMultisampleState = &ms; pi.pColorBlendState = &cb;
        pi.layout = pipelineLayout; pi.renderPass = renderPass;
        vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pi, nullptr, &graphicsPipeline);

        vkDestroyShaderModule(device, vert, nullptr);
        vkDestroyShaderModule(device, frag, nullptr);
    }

    void createFramebuffers() {
        swapChainFramebuffers.resize(swapChainImageViews.size());
        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            VkFramebufferCreateInfo ci{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
            ci.renderPass = renderPass; ci.attachmentCount = 1;
            ci.pAttachments = &swapChainImageViews[i];
            ci.width = swapChainExtent.width; ci.height = swapChainExtent.height; ci.layers = 1;
            vkCreateFramebuffer(device, &ci, nullptr, &swapChainFramebuffers[i]);
        }
    }

    void createCommandPool() {
        VkCommandPoolCreateInfo ci{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        ci.queueFamilyIndex = findQueueFamilies(physicalDevice).graphicsFamily.value();
        vkCreateCommandPool(device, &ci, nullptr, &commandPool);
    }

    void createCommandBuffers() {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        VkCommandBufferAllocateInfo ai{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        ai.commandPool = commandPool; ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        ai.commandBufferCount = MAX_FRAMES_IN_FLIGHT;
        vkAllocateCommandBuffers(device, &ai, commandBuffers.data());
    }

    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        VkSemaphoreCreateInfo si{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        VkFenceCreateInfo fi{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        fi.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkCreateSemaphore(device, &si, nullptr, &imageAvailableSemaphores[i]);
            vkCreateSemaphore(device, &si, nullptr, &renderFinishedSemaphores[i]);
            vkCreateFence(device, &fi, nullptr, &inFlightFences[i]);
        }
    }

    void initImGui() {
        VkDescriptorPoolSize ps{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000};
        VkDescriptorPoolCreateInfo pi{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        pi.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pi.maxSets = 1000; pi.poolSizeCount = 1; pi.pPoolSizes = &ps;
        vkCreateDescriptorPool(device, &pi, nullptr, &imguiDescriptorPool);

        IMGUI_CHECKVERSION(); ImGui::CreateContext(); ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo ii{};
        ii.Instance = instance; ii.PhysicalDevice = physicalDevice; ii.Device = device;
        ii.QueueFamily = findQueueFamilies(physicalDevice).graphicsFamily.value();
        ii.Queue = graphicsQueue; ii.DescriptorPool = imguiDescriptorPool;
        ii.RenderPass = renderPass; ii.MinImageCount = 2;
        ii.ImageCount = (uint32_t)swapChainImages.size(); ii.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        ImGui_ImplVulkan_Init(&ii); ImGui_ImplVulkan_CreateFontsTexture();
    }

    void drawFrame() {
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        uint32_t imgIdx;
        vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imgIdx);
        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        ImGui_ImplVulkan_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();
        ImGui::Begin("Procedural Textures");
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        const char* types[] = {"Checkerboard", "Circles", "Stripes", "Procedural"};
        int t = (int)pushConstants.textureType;
        if (ImGui::Combo("Type", &t, types, 4)) pushConstants.textureType = (float)t;
        ImGui::SliderFloat("Scale", &pushConstants.scale, 1.0f, 20.0f);
        ImGui::SliderFloat("Param", &pushConstants.param1, -3.14f, 3.14f);
        ImGui::End();
        ImGui::Render();

        VkCommandBuffer cmd = commandBuffers[currentFrame];
        vkResetCommandBuffer(cmd, 0);
        VkCommandBufferBeginInfo bi{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        vkBeginCommandBuffer(cmd, &bi);
        VkClearValue cv{{{0,0,0,1}}};
        VkRenderPassBeginInfo rpi{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
        rpi.renderPass = renderPass; rpi.framebuffer = swapChainFramebuffers[imgIdx];
        rpi.renderArea.extent = swapChainExtent; rpi.clearValueCount = 1; rpi.pClearValues = &cv;
        vkCmdBeginRenderPass(cmd, &rpi, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstants), &pushConstants);
        vkCmdDraw(cmd, 6, 1, 0, 0);
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
        vkCmdEndRenderPass(cmd);
        vkEndCommandBuffer(cmd);

        VkSemaphore wait[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSemaphore signal[] = {renderFinishedSemaphores[currentFrame]};
        VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};
        si.waitSemaphoreCount = 1; si.pWaitSemaphores = wait; si.pWaitDstStageMask = stages;
        si.commandBufferCount = 1; si.pCommandBuffers = &cmd;
        si.signalSemaphoreCount = 1; si.pSignalSemaphores = signal;
        vkQueueSubmit(graphicsQueue, 1, &si, inFlightFences[currentFrame]);

        VkPresentInfoKHR pri{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
        pri.waitSemaphoreCount = 1; pri.pWaitSemaphores = signal;
        pri.swapchainCount = 1; pri.pSwapchains = &swapChain; pri.pImageIndices = &imgIdx;
        vkQueuePresentKHR(presentQueue, &pri);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice dev) {
        QueueFamilyIndices indices;
        uint32_t count; vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, nullptr);
        std::vector<VkQueueFamilyProperties> props(count);
        vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, props.data());
        for (uint32_t i = 0; i < count; i++) {
            if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily = i;
            VkBool32 present; vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &present);
            if (present) indices.presentFamily = i;
            if (indices.isComplete()) break;
        }
        return indices;
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice dev) {
        SwapChainSupportDetails d;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &d.capabilities);
        uint32_t fc; vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &fc, nullptr);
        d.formats.resize(fc); vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &fc, d.formats.data());
        uint32_t pc; vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &pc, nullptr);
        d.presentModes.resize(pc); vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &pc, d.presentModes.data());
        return d;
    }
};

int main() {
    TextureApp app;
    try { app.run(); }
    catch (const std::exception& e) { std::cerr << e.what() << "\n"; return 1; }
    return 0;
}
