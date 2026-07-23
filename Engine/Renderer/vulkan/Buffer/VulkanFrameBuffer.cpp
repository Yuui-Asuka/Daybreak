#include "VulkanFramebuffer.h"

#include <stdexcept>
#include <iostream>

namespace Daybreak
{

    void VulkanFramebuffer::Init(
        VkDevice device,
        VkRenderPass renderPass,
        const std::vector<VkImageView>& imageViews,
        VkExtent2D extent,
        VkImageView depthImageView)
    {
        /*
            保存Logical Device。

            Framebuffer属于Device资源。

            创建和销毁Framebuffer都需要VkDevice。
        */
        m_Device = device;


        /*
            根据Swapchain ImageView创建Framebuffer。

            一个Swapchain Image对应一个Framebuffer。

            例如：

                Swapchain Image Count = 3


                ImageView[0]
                     |
                     v
                Framebuffer[0]


                ImageView[1]
                     |
                     v
                Framebuffer[1]


                ImageView[2]
                     |
                     v
                Framebuffer[2]

        */
        CreateFramebuffers(
            renderPass,
            imageViews,
            extent,
            depthImageView
        );
    }



    void VulkanFramebuffer::CreateFramebuffers(
        VkRenderPass renderPass,
        const std::vector<VkImageView>& imageViews,
        VkExtent2D extent,
        VkImageView depthImageView
    )
    {
        /*
            Framebuffer数量和Swapchain Image数量一致。

            每张Swapchain Image都有自己的Framebuffer。
        */
        m_Framebuffers.resize(
            imageViews.size()
        );


        for (size_t i = 0;
            i < imageViews.size();
            i++)
        {

            /*
                Framebuffer Attachment列表。

            */
            VkImageView attachments[] =
            {
                imageViews[i],
                depthImageView
            };



            /*
                Framebuffer Create Info

                描述：

                    使用哪个RenderPass

                    使用哪些ImageView

                    宽高是多少


            */
            VkFramebufferCreateInfo framebufferInfo{};


            framebufferInfo.sType =
                VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;



            /*
                必须和Pipeline创建时使用的RenderPass一致。

                Pipeline:

                    renderPass A


                Framebuffer:

                    renderPass A


                才能绑定。

            */
            framebufferInfo.renderPass =
                renderPass;



            /*
                Attachment数量。

                当前只有Color Attachment。

            */
            framebufferInfo.attachmentCount =
                2;


            framebufferInfo.pAttachments =
                attachments;



            /*
                Framebuffer尺寸。

                通常等于Swapchain Extent。

            */
            framebufferInfo.width =
                extent.width;

            framebufferInfo.height =
                extent.height;



            /*
                Layer数量。

                普通2D窗口：

                    1


                Cubemap / Array Texture:

                    >1

            */
            framebufferInfo.layers =
                1;



            if (vkCreateFramebuffer(
                m_Device,
                &framebufferInfo,
                nullptr,
                &m_Framebuffers[i])
                != VK_SUCCESS)
            {
                throw std::runtime_error(
                    "Failed to create framebuffer!"
                );
            }
        }


        std::cout
            << "Framebuffer Count: "
            << m_Framebuffers.size()
            << std::endl;
    }



    void VulkanFramebuffer::Shutdown()
    {
        /*
            销毁Framebuffer。

            创建顺序：

                Swapchain

                    |

                ImageView

                    |

                Framebuffer


            销毁顺序相反：

                Framebuffer

                    |

                ImageView

                    |

                Swapchain

        */

        for (VkFramebuffer framebuffer :
        m_Framebuffers)
        {
            if (framebuffer != VK_NULL_HANDLE)
            {
                vkDestroyFramebuffer(
                    m_Device,
                    framebuffer,
                    nullptr
                );
            }
        }


        m_Framebuffers.clear();
    }

}