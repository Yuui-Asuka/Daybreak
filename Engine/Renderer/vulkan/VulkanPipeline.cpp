#include "VulkanPipeline.h"
#include "../Mesh/Vertex.h"

#include <stdexcept>
#include <iostream>


namespace Daybreak
{

    /**
     * @brief Initializes the Vulkan graphics pipeline.
     *
     * Stores the Vulkan device handle and creates the graphics pipeline.
     *
     * @param device Vulkan logical device used for pipeline creation.
     * @param extent Render target size.
     * @param renderPass Render pass associated with this pipeline.
     * @param vertexShader Vertex shader module.
     * @param fragmentShader Fragment shader module.
     * @param descriptorSetLayout Descriptor set layout used by shaders.
     */
    void VulkanPipeline::Init(
        VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        VkShaderModule vertexShader,
        VkShaderModule fragmentShader,
        VkDescriptorSetLayout descriptorSetLayout
    )
    {
        // Store Vulkan logical device.
        m_Device = device;


        // Create graphics pipeline.
        CreatePipeline(
            device,
            extent,
            renderPass,
            vertexShader,
            fragmentShader,
            descriptorSetLayout
        );
    }



    /**
     * @brief Creates the Vulkan graphics pipeline.
     *
     * Configures all fixed-function pipeline states and combines
     * shader stages into a graphics pipeline object.
     *
     * @param device Vulkan logical device.
     * @param extent Render target size.
     * @param renderPass Render pass used by the pipeline.
     * @param vertexShader Vertex shader module.
     * @param fragmentShader Fragment shader module.
     * @param descriptorSetLayout Descriptor layout accessible by shaders.
     */
    void VulkanPipeline::CreatePipeline(
        VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        VkShaderModule vertexShader,
        VkShaderModule fragmentShader,
        VkDescriptorSetLayout descriptorSetLayout
    )
    {

        // Configure vertex shader stage.
        VkPipelineShaderStageCreateInfo vertStage{};

        vertStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        vertStage.stage =
            VK_SHADER_STAGE_VERTEX_BIT;

        vertStage.module =
            vertexShader;

        vertStage.pName =
            "main";


        // Configure fragment shader stage.
        VkPipelineShaderStageCreateInfo fragStage{};

        fragStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        fragStage.stage =
            VK_SHADER_STAGE_FRAGMENT_BIT;

        fragStage.module =
            fragmentShader;

        fragStage.pName =
            "main";


        VkPipelineShaderStageCreateInfo shaderStages[] =
        {
            vertStage,
            fragStage
        };



        // Configure vertex buffer layout.
        VkVertexInputBindingDescription binding =
            Vertex::GetBindingDescription();


        VkVertexInputAttributeDescription attributes[3];

        Vertex::GetAttributeDescriptions(
            attributes
        );


        VkPipelineVertexInputStateCreateInfo vertexInput{};

        vertexInput.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;


        vertexInput.vertexBindingDescriptionCount =
            1;

        vertexInput.pVertexBindingDescriptions =
            &binding;


        vertexInput.vertexAttributeDescriptionCount =
            3;

        vertexInput.pVertexAttributeDescriptions =
            attributes;



        // Configure primitive assembly.
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

        inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;


        inputAssembly.topology =
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;


        inputAssembly.primitiveRestartEnable =
            VK_FALSE;



        // Configure viewport.
        VkViewport viewport{};

        viewport.x = 0.0f;

        viewport.y = 0.0f;

        viewport.width =
            static_cast<float>(extent.width);

        viewport.height =
            static_cast<float>(extent.height);

        viewport.minDepth = 0.0f;

        viewport.maxDepth = 1.0f;



        // Configure rendering area.
        VkRect2D scissor{};

        scissor.offset =
        {
            0,
            0
        };

        scissor.extent =
            extent;



        VkPipelineViewportStateCreateInfo viewportState{};

        viewportState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;


        viewportState.viewportCount =
            1;

        viewportState.pViewports =
            &viewport;


        viewportState.scissorCount =
            1;

        viewportState.pScissors =
            &scissor;



        // Configure rasterization state.
        VkPipelineRasterizationStateCreateInfo rasterizer{};

        rasterizer.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;


        rasterizer.depthClampEnable =
            VK_FALSE;


        rasterizer.rasterizerDiscardEnable =
            VK_FALSE;


        rasterizer.polygonMode =
            VK_POLYGON_MODE_FILL;


        rasterizer.lineWidth =
            1.0f;


        rasterizer.cullMode =
            VK_CULL_MODE_NONE;


        rasterizer.frontFace =
            VK_FRONT_FACE_CLOCKWISE;

        // Configure depth testing state.
        VkPipelineDepthStencilStateCreateInfo depthStencil{};

        depthStencil.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;


        depthStencil.depthTestEnable =
            VK_TRUE;


        depthStencil.depthWriteEnable =
            VK_TRUE;


        depthStencil.depthCompareOp =
            VK_COMPARE_OP_LESS;


        depthStencil.depthBoundsTestEnable =
            VK_FALSE;


        depthStencil.stencilTestEnable =
            VK_FALSE;



        // Configure multisampling state.
        VkPipelineMultisampleStateCreateInfo multisampling{};

        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;


        multisampling.sampleShadingEnable =
            VK_FALSE;


        multisampling.rasterizationSamples =
            VK_SAMPLE_COUNT_1_BIT;



        // Configure framebuffer color blending.
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};

        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;


        colorBlendAttachment.blendEnable =
            VK_FALSE;



        VkPipelineColorBlendStateCreateInfo colorBlending{};

        colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;


        colorBlending.logicOpEnable =
            VK_FALSE;


        colorBlending.attachmentCount =
            1;


        colorBlending.pAttachments =
            &colorBlendAttachment;



        // Configure shader resource layout.
        VkPipelineLayoutCreateInfo layoutInfo{};

        layoutInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;


        layoutInfo.setLayoutCount =
            1;


        layoutInfo.pSetLayouts =
            &descriptorSetLayout;



        // Create pipeline layout.
        if (vkCreatePipelineLayout(
            device,
            &layoutInfo,
            nullptr,
            &m_PipelineLayout)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create pipeline layout!"
            );
        }



        // Combine all pipeline states into a graphics pipeline.
        VkGraphicsPipelineCreateInfo pipelineInfo{};

        pipelineInfo.sType =
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;


        pipelineInfo.stageCount =
            2;


        pipelineInfo.pStages =
            shaderStages;


        pipelineInfo.pVertexInputState =
            &vertexInput;


        pipelineInfo.pInputAssemblyState =
            &inputAssembly;


        pipelineInfo.pViewportState =
            &viewportState;


        pipelineInfo.pRasterizationState =
            &rasterizer;


        pipelineInfo.pMultisampleState =
            &multisampling;


        pipelineInfo.pDepthStencilState =
            &depthStencil;


        pipelineInfo.pColorBlendState =
            &colorBlending;


        pipelineInfo.layout =
            m_PipelineLayout;


        pipelineInfo.renderPass =
            renderPass;


        pipelineInfo.subpass =
            0;



        // Create Vulkan graphics pipeline.
        if (vkCreateGraphicsPipelines(
            m_Device,
            VK_NULL_HANDLE,
            1,
            &pipelineInfo,
            nullptr,
            &m_Pipeline)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create graphics pipeline!"
            );
        }


        std::cout
            << "Graphics Pipeline Created!"
            << std::endl;

    }



    /**
     * @brief Releases graphics pipeline resources.
     *
     * Destroys:
     *
     * - VkPipeline
     * - VkPipelineLayout
     *
     * Resources depending on the pipeline must be released before
     * calling this function.
     */
    void VulkanPipeline::Shutdown()
    {

        // Destroy graphics pipeline.
        if (m_Pipeline != VK_NULL_HANDLE)
        {
            vkDestroyPipeline(
                m_Device,
                m_Pipeline,
                nullptr
            );


            m_Pipeline =
                VK_NULL_HANDLE;
        }



        // Destroy pipeline layout.
        if (m_PipelineLayout != VK_NULL_HANDLE)
        {
            vkDestroyPipelineLayout(
                m_Device,
                m_PipelineLayout,
                nullptr
            );


            m_PipelineLayout =
                VK_NULL_HANDLE;
        }

    }


}