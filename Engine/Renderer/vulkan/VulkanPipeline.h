#pragma once

#include <vulkan/vulkan.h>

namespace Daybreak
{

    /*
        VulkanPipeline

        管理 Vulkan Graphics Pipeline。

        Pipeline 描述 GPU 如何执行一次绘制：

        Shader
            ↓
        Vertex Input
            ↓
        Input Assembly
            ↓
        Rasterizer
            ↓
        Fragment Output
    */
    class VulkanPipeline
    {
    public:

        /*
            创建 Graphics Pipeline

            device:
                Vulkan Logical Device

            extent:
                Swapchain尺寸

            renderPass:
                Pipeline绑定的RenderPass

            vertShader:
                Vertex Shader Module

            fragShader:
                Fragment Shader Module
        */
        void Init(
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderPass,
            VkShaderModule vertShader,
            VkShaderModule fragShader
        );

        /*
            销毁Pipeline资源
        */
        void Shutdown();

        /*
            获取Graphics Pipeline
        */
        VkPipeline GetPipeline() const
        {
            return m_Pipeline;
        }

        /*
            获取Pipeline Layout
        */
        VkPipelineLayout GetPipelineLayout() const
        {
            return m_PipelineLayout;
        }

    private:

        /*
            创建Graphics Pipeline

            包含：

            - Shader Stage
            - Vertex Input
            - Input Assembly
            - Viewport
            - Rasterizer
            - Multisampling
            - Color Blend
        */
        void CreatePipeline(
            VkExtent2D extent,
            VkRenderPass renderPass,
            VkShaderModule vertShader,
            VkShaderModule fragShader
        );

    private:

        // Vulkan Logical Device
        VkDevice m_Device = VK_NULL_HANDLE;

        // Graphics Pipeline
        VkPipeline m_Pipeline = VK_NULL_HANDLE;

        // Pipeline Layout
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
    };

}