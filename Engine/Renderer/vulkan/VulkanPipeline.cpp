#include "VulkanPipeline.h"
#include "../Mesh/Vertex.h"

#include <stdexcept>
#include <iostream>


namespace Daybreak
{

    void VulkanPipeline::Init(
        VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        VkShaderModule vertexShader,
        VkShaderModule fragmentShader,
        VkDescriptorSetLayout descriptorSetLayout
    )
    {
        // 保存 Vulkan Logical Device
        m_Device = device;

        CreatePipeline(
            device,
            extent,
            renderPass,
            vertexShader,
            fragmentShader,
            descriptorSetLayout
        );
    }


    void VulkanPipeline::CreatePipeline(
        VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        VkShaderModule vertexShader,
        VkShaderModule fragmentShader,
        VkDescriptorSetLayout descriptorSetLayout
    )
    {

        /*
            Shader Stage

            定义Pipeline使用哪些Shader。

            当前：

            Vertex Shader
                负责顶点处理

            Fragment Shader
                负责像素颜色计算

        */

        VkPipelineShaderStageCreateInfo vertStage{};

        vertStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        vertStage.stage =
            VK_SHADER_STAGE_VERTEX_BIT;

        vertStage.module =
            vertexShader;

        vertStage.pName =
            "main";


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


        /*
            Vertex Input

            描述顶点数据如何进入Vertex Shader。


            当前三角形：

            没有Vertex Buffer。

            顶点数据由Shader内部生成。

            所以：

            binding = 0

            attribute = 0

        */

        // =========================
        // Vertex Input
        // =========================

        VkVertexInputBindingDescription binding =
            Vertex::GetBindingDescription();

        VkVertexInputAttributeDescription attributes[3];

        Vertex::GetAttributeDescriptions(
            attributes
        );

        VkPipelineVertexInputStateCreateInfo vertexInput{};

        vertexInput.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        // 一个 Vertex Binding
        vertexInput.vertexBindingDescriptionCount =
            1;

        vertexInput.pVertexBindingDescriptions =
            &binding;

        // 三个 Attribute
        vertexInput.vertexAttributeDescriptionCount =
            3;

        vertexInput.pVertexAttributeDescriptions =
            attributes;


        /*
            Input Assembly

            定义顶点如何组成图元。


            TRIANGLE_LIST:

            每三个顶点组成一个三角形。

        */

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

        inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

        inputAssembly.topology =
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        inputAssembly.primitiveRestartEnable =
            VK_FALSE;



        /*
            Viewport

            定义渲染区域。

            通常与Swapchain尺寸一致。

        */

        VkViewport viewport{};

        viewport.x = 0.0f;

        viewport.y = 0.0f;

        viewport.width =
            static_cast<float>(extent.width);

        viewport.height =
            static_cast<float>(extent.height);

        viewport.minDepth = 0.0f;

        viewport.maxDepth = 1.0f;



        /*
            Scissor

            定义实际允许写入的区域。

            当前覆盖整个窗口。

        */

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

        viewportState.viewportCount = 1;

        viewportState.pViewports =
            &viewport;

        viewportState.scissorCount = 1;

        viewportState.pScissors =
            &scissor;



        /*
            Rasterizer

            光栅化阶段：

            三角形
                |
                v
            像素Fragment


            当前：

            填充模式

            开启背面剔除

        */

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



        /*
            Multisampling

            控制抗锯齿。

            当前：

            不开启MSAA。

        */

        VkPipelineMultisampleStateCreateInfo multisampling{};

        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

        multisampling.sampleShadingEnable =
            VK_FALSE;

        multisampling.rasterizationSamples =
            VK_SAMPLE_COUNT_1_BIT;



        /*
            Color Blend

            控制Fragment颜色如何写入Framebuffer。


            当前：

            直接覆盖。

        */

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



        /*
            Pipeline Layout

            描述Shader可以访问的资源：

            - Descriptor Set
            - Push Constant


            当前没有外部资源。

            创建空Layout。

        */

        VkPipelineLayoutCreateInfo layoutInfo{};


        layoutInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;


        layoutInfo.setLayoutCount = 1;


        layoutInfo.pSetLayouts =
            &descriptorSetLayout;



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



        /*
            创建Graphics Pipeline


            将前面的所有状态组合成最终Pipeline对象。

        */

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

        pipelineInfo.pColorBlendState =
            &colorBlending;

        pipelineInfo.layout =
            m_PipelineLayout;

        pipelineInfo.renderPass =
            renderPass;

        pipelineInfo.subpass =
            0;



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


    void VulkanPipeline::Shutdown()
    {
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