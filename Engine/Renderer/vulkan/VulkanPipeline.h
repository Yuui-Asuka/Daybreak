#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @class VulkanPipeline
     *
     * @brief Manages Vulkan graphics pipeline resources.
     *
     * A graphics pipeline defines how the GPU processes rendering commands.
     *
     * Pipeline stages:
     *
     * Shader
     *     |
     *     v
     * Vertex Input
     *     |
     *     v
     * Input Assembly
     *     |
     *     v
     * Rasterization
     *     |
     *     v
     * Fragment Output
     */
    class VulkanPipeline
    {
    public:


        /**
         * @brief Initializes the graphics pipeline.
         *
         * Creates a Vulkan graphics pipeline using the specified
         * shaders, render pass, and pipeline layout.
         *
         * @param device Vulkan logical device used for pipeline creation.
         * @param extent Render target size.
         * @param renderPass Render pass associated with the pipeline.
         * @param vertexShader Vertex shader module.
         * @param fragmentShader Fragment shader module.
         * @param descriptorSetLayout Descriptor set layout accessible by shaders.
         */
        void Init(
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderPass,
            VkShaderModule vertexShader,
            VkShaderModule fragmentShader,
            VkDescriptorSetLayout descriptorSetLayout
        );


        /**
         * @brief Releases graphics pipeline resources.
         *
         * Destroys the graphics pipeline and pipeline layout.
         */
        void Shutdown();



        /**
         * @brief Retrieves the graphics pipeline.
         *
         * @return VkPipeline Graphics pipeline handle.
         */
        VkPipeline GetPipeline() const
        {
            return m_Pipeline;
        }



        /**
         * @brief Retrieves the pipeline layout.
         *
         * The pipeline layout describes resources accessible by shaders,
         * such as descriptor sets and push constants.
         *
         * @return VkPipelineLayout Pipeline layout handle.
         */
        VkPipelineLayout GetPipelineLayout() const
        {
            return m_PipelineLayout;
        }



    private:


        /**
         * @brief Creates the Vulkan graphics pipeline.
         *
         * Configures:
         *
         * - Shader stages
         * - Vertex input
         * - Input assembly
         * - Viewport
         * - Rasterization
         * - Multisampling
         * - Color blending
         *
         * @param device Vulkan logical device used for creation.
         * @param extent Render target size.
         * @param renderPass Render pass associated with the pipeline.
         * @param vertexShader Vertex shader module.
         * @param fragmentShader Fragment shader module.
         * @param descriptorSetLayout Shader resource layout.
         */
        void CreatePipeline(
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderPass,
            VkShaderModule vertexShader,
            VkShaderModule fragmentShader,
            VkDescriptorSetLayout descriptorSetLayout
        );


    private:


        /**
         * @brief Vulkan logical device handle.
         *
         * Used to create and destroy pipeline resources.
         */
        VkDevice m_Device = VK_NULL_HANDLE;



        /**
         * @brief Vulkan graphics pipeline handle.
         *
         * Defines the GPU rendering state used for drawing.
         */
        VkPipeline m_Pipeline = VK_NULL_HANDLE;



        /**
         * @brief Vulkan pipeline layout handle.
         *
         * Describes resources available to shaders.
         */
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

    };

}