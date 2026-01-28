/**
 * Chapter 02-02: Fragment Gradients
 *
 * Fragment Shader에서 다양한 그라데이션 효과를 구현합니다.
 * - Linear: 선형 그라데이션
 * - Radial: 방사형 그라데이션
 * - Angular: 각도 기반 그라데이션
 * - Diamond: 다이아몬드 패턴
 * - Noise: 노이즈 기반 효과
 *
 * ShaderExampleBase를 상속받아 셰이더 로직에만 집중합니다.
 */

#include <shader_example_base.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>

#include <glm/glm.hpp>

#include <chrono>
#include <iostream>
#include <cmath>

// Push constant structure for fragment shader
struct PushConstants
{
    float time;
    float gradientType;   // 0-4: different gradient types
    float colorMix;       // Color interpolation factor
    float param1;         // Extra parameter (rotation, scale, etc.)
    float color1[3];      // First color (RGB)
    float padding1;
    float color2[3];      // Second color (RGB)
    float padding2;
};

class FragmentGradientsExample : public ch02::ShaderExampleBase
{
public:
    FragmentGradientsExample()
        : ShaderExampleBase("Chapter 02-02: Fragment Gradients", 800, 600)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

protected:
    void createGraphicsPipeline() override
    {
        auto vertShaderCode = readFile("shaders/vert.spv");
        auto fragShaderCode = readFile("shaders/frag.spv");

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkViewport viewport{};
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.extent = swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_NONE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        // Push constant for both vertex and fragment shaders
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstants);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create pipeline layout!");

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
            throw std::runtime_error("Failed to create graphics pipeline!");

        vkDestroyShaderModule(device, fragShaderModule, nullptr);
        vkDestroyShaderModule(device, vertShaderModule, nullptr);
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) override
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin recording command buffer!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        // Push constants
        vkCmdPushConstants(commandBuffer, pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(PushConstants), &pushConstants);

        // Draw fullscreen quad (2 triangles = 6 vertices)
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);

        // ImGui
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("Failed to record command buffer!");
    }

    void renderImGui() override
    {
        ImGui::Begin("Fragment Gradients");
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Separator();

        const char* gradientTypes[] = { "Linear", "Radial", "Angular", "Diamond", "Noise" };
        int gradType = static_cast<int>(pushConstants.gradientType);
        if (ImGui::Combo("Gradient Type", &gradType, gradientTypes, IM_ARRAYSIZE(gradientTypes)))
            pushConstants.gradientType = static_cast<float>(gradType);

        ImGui::SliderFloat("Mix", &pushConstants.colorMix, 0.0f, 1.0f);
        ImGui::SliderFloat("Param", &pushConstants.param1, -3.14159f, 3.14159f);

        ImGui::Separator();
        ImGui::ColorEdit3("Color 1", pushConstants.color1);
        ImGui::ColorEdit3("Color 2", pushConstants.color2);

        ImGui::Checkbox("Animate Colors", &animateColors);

        ImGui::Text("Time: %.2f s", pushConstants.time);
        ImGui::End();
    }

    void onUpdate() override
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        pushConstants.time = std::chrono::duration<float>(currentTime - startTime).count();

        if (animateColors)
        {
            float t = pushConstants.time * 0.5f;
            pushConstants.colorMix = (std::sin(t) + 1.0f) * 0.5f;
        }
    }

private:
    PushConstants pushConstants = {
        0.0f, 0.0f, 0.5f, 0.0f,
        {1.0f, 0.2f, 0.3f}, 0.0f,  // Red-ish
        {0.2f, 0.3f, 1.0f}, 0.0f   // Blue-ish
    };
    std::chrono::high_resolution_clock::time_point startTime;
    bool animateColors = true;
};

int main()
{
    FragmentGradientsExample app;
    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
