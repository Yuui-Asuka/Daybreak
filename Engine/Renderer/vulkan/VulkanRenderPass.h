#pragma once

#include <vulkan/vulkan.h>

namespace Daybreak
{

    /*
        VulkanRenderPass

        管理 Vulkan RenderPass。

        RenderPass 描述一次渲染过程：

            Attachment
                |
                |
                v
            Subpass
                |
                |
                v
            Render Pipeline


        它定义：

        - 渲染目标格式
        - Color Attachment
        - Depth Attachment
        - Load / Store 操作
        - Layout转换


        当前项目：

        只有一个Color Attachment。

        渲染流程：

            Swapchain Image

                  |

            RenderPass

                  |

            Framebuffer


        生命周期：

            Init()

                |
                v

            创建 VkRenderPass


            Shutdown()

                |
                v

            销毁 VkRenderPass

    */
    class VulkanRenderPass
    {
    public:

        /*
            创建RenderPass。


            device:

                Vulkan Logical Device。


            imageFormat:

                Swapchain Image Format。

                RenderPass的Color Attachment
                必须与Swapchain格式匹配。


        */
        void Init(
            VkDevice device,
            VkFormat imageFormat
        );


        /*
            销毁RenderPass资源。
        */
        void Shutdown();


        /*
            获取VkRenderPass句柄。


            创建Graphics Pipeline时：

                VkGraphicsPipelineCreateInfo::renderPass


            需要绑定这个对象。


        */
        VkRenderPass GetRenderPass() const
        {
            return m_RenderPass;
        }


    private:

        /*
            Vulkan Logical Device。


            RenderPass属于Device资源。

            创建和销毁都需要Device。
        */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /*
            Vulkan RenderPass对象。
        */
        VkRenderPass m_RenderPass =
            VK_NULL_HANDLE;

    };

}