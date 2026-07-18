#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

#include "VulkanRenderPass.h"

namespace Daybreak
{

    void VulkanRenderPass::Init(
        VkDevice device,
        VkFormat imageFormat)
    {
        // 保存Logical Device
        // RenderPass的创建和销毁都需要Device
        m_Device = device;


        /*
            Attachment Description

            描述RenderPass使用的渲染目标。

            当前：

            一个Color Attachment。

            对应Swapchain Image。


            渲染流程：

            Swapchain Image

                  |
                  v

            Color Attachment

                  |
                  v

            Present到屏幕

        */

        VkAttachmentDescription colorAttachment{};


        // 颜色格式必须和Swapchain一致
        colorAttachment.format =
            imageFormat;


        // 当前不使用MSAA
        colorAttachment.samples =
            VK_SAMPLE_COUNT_1_BIT;


        /*
            Load Operation

            RenderPass开始时如何处理已有数据。


            CLEAR:

                开始渲染前清空颜色。

                使用VkClearValue中的颜色。


        */
        colorAttachment.loadOp =
            VK_ATTACHMENT_LOAD_OP_CLEAR;


        /*
            Store Operation

            RenderPass结束后如何处理结果。


            STORE:

                保存渲染结果。

                因为之后需要Present到屏幕。

        */
        colorAttachment.storeOp =
            VK_ATTACHMENT_STORE_OP_STORE;


        /*
            Stencil Attachment

            当前没有Stencil。

            所以忽略。

        */
        colorAttachment.stencilLoadOp =
            VK_ATTACHMENT_LOAD_OP_DONT_CARE;

        colorAttachment.stencilStoreOp =
            VK_ATTACHMENT_STORE_OP_DONT_CARE;



        /*
            Image Layout Transition


            初始状态：

                undefined

                GPU不关心之前内容。


            最终状态：

                present

                交给Swapchain显示。

        */
        colorAttachment.initialLayout =
            VK_IMAGE_LAYOUT_UNDEFINED;

        colorAttachment.finalLayout =
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;



        /*
            Attachment Reference


            Subpass引用Attachment。


            attachment = 0

            表示使用colorAttachment数组中的第0个。

        */

        VkAttachmentReference colorAttachmentRef{};

        colorAttachmentRef.attachment = 0;


        /*
            Shader输出颜色时，

            Image需要处于：

                COLOR_ATTACHMENT_OPTIMAL

            Layout。

        */
        colorAttachmentRef.layout =
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;



        /*
            Subpass


            描述一次渲染步骤。


            当前：

            一个Graphics Subpass。


            Pipeline:

                Vertex Shader

                    |

                Fragment Shader

                    |

                Color Attachment


        */

        VkSubpassDescription subpass{};


        subpass.pipelineBindPoint =
            VK_PIPELINE_BIND_POINT_GRAPHICS;


        // 使用一个Color Attachment
        subpass.colorAttachmentCount = 1;


        subpass.pColorAttachments =
            &colorAttachmentRef;



        /*
            Subpass Dependency


            描述不同Pipeline阶段之间的同步。


            当前：

            外部操作

                ↓

            Color Attachment Output


            确保写入Framebuffer前资源准备完成。

        */

        VkSubpassDependency dependency{};


        // RenderPass之前的操作
        dependency.srcSubpass =
            VK_SUBPASS_EXTERNAL;


        // 当前Subpass
        dependency.dstSubpass = 0;


        /*
            Pipeline Stage同步点

            等待Color Attachment Output阶段。

        */
        dependency.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        dependency.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;



        /*
            Access Mask


            外部阶段没有访问。


            当前Subpass需要写Color Attachment。

        */
        dependency.srcAccessMask = 0;

        dependency.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;



        /*
            RenderPass Create Info


            最终把：

                Attachment

                Subpass

                Dependency


            组合成VkRenderPass。

        */

        VkRenderPassCreateInfo renderPassInfo{};


        renderPassInfo.sType =
            VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;


        // Attachment数量
        renderPassInfo.attachmentCount = 1;

        renderPassInfo.pAttachments =
            &colorAttachment;



        // Subpass数量
        renderPassInfo.subpassCount = 1;

        renderPassInfo.pSubpasses =
            &subpass;



        // Dependency数量
        renderPassInfo.dependencyCount = 1;

        renderPassInfo.pDependencies =
            &dependency;



        if (vkCreateRenderPass(
            m_Device,
            &renderPassInfo,
            nullptr,
            &m_RenderPass)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create RenderPass!"
            );
        }


        std::cout
            << "RenderPass Created!"
            << std::endl;
    }



    void VulkanRenderPass::Shutdown()
    {
        if (m_RenderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(
                m_Device,
                m_RenderPass,
                nullptr
            );

            m_RenderPass =
                VK_NULL_HANDLE;
        }
    }

}