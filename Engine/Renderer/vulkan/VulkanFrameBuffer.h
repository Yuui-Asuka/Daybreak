#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace Daybreak
{

    /*
        VulkanFramebuffer

        管理 Vulkan Framebuffer。


        Framebuffer 是 RenderPass 的具体渲染目标。


        关系：

            Swapchain Image

                  |
                  v

            ImageView

                  |
                  v

            Framebuffer

                  |
                  v

            RenderPass


        RenderPass描述：

            "怎么渲染"


        Framebuffer描述：

            "渲染到哪张图片"


        当前结构：

            一个Swapchain Image

                    |

            一个ImageView

                    |

            一个Framebuffer



        Swapchain通常有多个Image：

            Image[0]
                |
            Framebuffer[0]


            Image[1]
                |
            Framebuffer[1]


            Image[2]
                |
            Framebuffer[2]


    */
    class VulkanFramebuffer
    {
    public:

        /*
            创建Framebuffer。


            device:

                Vulkan Logical Device。


            renderPass:

                与Framebuffer匹配的RenderPass。


                创建Pipeline时也使用同一个RenderPass。


            imageViews:

                Swapchain Image Views。


                每个ImageView创建一个Framebuffer。


            extent:

                Framebuffer宽高。


        */
        void Init(
            VkDevice device,
            VkRenderPass renderPass,
            const std::vector<VkImageView>& imageViews,
            VkExtent2D extent
        );


        /*
            销毁所有Framebuffer。
        */
        void Shutdown();


        /*
            获取Framebuffer列表。


            CommandBuffer录制时：

                vkCmdBeginRenderPass()


            需要指定当前Framebuffer。

        */
        const std::vector<VkFramebuffer>& GetFramebuffers() const
        {
            return m_Framebuffers;
        }


    private:

        /*
            根据Swapchain ImageView创建Framebuffer。

            一个ImageView对应一个Framebuffer。


            例如：

                Swapchain Image Count = 3


                创建：

                    Framebuffer[0]

                    Framebuffer[1]

                    Framebuffer[2]

        */
        void CreateFramebuffers(
            VkRenderPass renderPass,
            const std::vector<VkImageView>& imageViews,
            VkExtent2D extent
        );


    private:

        /*
            Vulkan Logical Device。

            Framebuffer属于Device资源。

        */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /*
            Swapchain对应的Framebuffer集合。

            数量通常等于Swapchain Image数量。

        */
        std::vector<VkFramebuffer> m_Framebuffers;

    };

}