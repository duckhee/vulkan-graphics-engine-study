#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <iostream>

namespace vk
{
    /**
     * VulkanPipeline 모듈
     *
     * 학습 목표:
     * 1. Graphics Pipeline의 구성 요소 이해
     * 2. Shader Module 로딩 방법
     * 3. Pipeline States (Rasterizer, Blending 등)
     * 4. Pipeline Layout의 역할
     *
     * Graphics Pipeline: GPU가 렌더링을 수행하는 방법을 정의
     * - Vertex Input: 정점 데이터 형식
     * - Shaders: Vertex, Fragment 등
     * - Rasterization: 폴리곤을 픽셀로 변환
     * - Color Blending: 최종 색상 계산
     *
     * Pipeline은 생성 비용이 높으므로 미리 생성하고 재사용
     */
    class VulkanPipeline
    {
    public:
        VulkanPipeline() = default;
        virtual ~VulkanPipeline();

        // Delete copy
        VulkanPipeline(const VulkanPipeline&) = delete;
        VulkanPipeline& operator=(const VulkanPipeline&) = delete;

        /**
         * 기본 Graphics Pipeline 생성 (삼각형용)
         * @param vertShaderPath Vertex Shader SPIR-V 파일 경로
         * @param fragShaderPath Fragment Shader SPIR-V 파일 경로
         */
        void createGraphicsPipeline(VkDevice device,
                                    VkRenderPass renderPass,
                                    VkExtent2D extent,
                                    const std::string& vertShaderPath,
                                    const std::string& fragShaderPath);

        /**
         * 정리
         */
        void destroy();

        // Getters
        VkPipeline getPipeline() const { return graphicsPipeline; }
        VkPipelineLayout getLayout() const { return pipelineLayout; }

        // Utility: Shader 파일 읽기
        static std::vector<char> readShaderFile(const std::string& filename);

    protected:
        VkDevice device = VK_NULL_HANDLE;  // Reference (not owned)
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkPipeline graphicsPipeline = VK_NULL_HANDLE;

    private:
        VkShaderModule createShaderModule(const std::vector<char>& code);
    };
}
