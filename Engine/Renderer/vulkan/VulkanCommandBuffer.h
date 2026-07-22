#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace Daybreak
{

    /*
        VulkanCommandBuffer

        管理 Vulkan Command Buffer。


        Command Buffer 是提交给 GPU 执行的命令列表。


        CPU记录：

            vkCmdBeginRenderPass()

                  |

            vkCmdBindPipeline()

                  |

            vkCmdDraw()

                  |

            vkCmdEndRenderPass()



        GPU执行：

            vkQueueSubmit()



        当前结构：

            一个Swapchain Image

                    |

                    v

            一个Framebuffer

                    |

                    v

            一个CommandBuffer



        例如：

            Swapchain Image Count = 3


            CommandBuffer[0]

                    对应

            Framebuffer[0]


            CommandBuffer[1]

                    对应

            Framebuffer[1]


            CommandBuffer[2]

                    对应

            Framebuffer[2]

    */
    class VulkanCommandBuffer
    {
    public:

        /*
            初始化Command Buffer。


            device:

                Vulkan Logical Device。


            commandPool:

                Command Buffer来源。


            renderPass:

                录制RenderPass命令时使用。


            extent:

                渲染区域大小。


            pipeline:

                绘制时绑定的Graphics Pipeline。


            framebuffers:

                每个Command Buffer对应一个Framebuffer。


        */
        void Init(
            VkDevice device,
            VkCommandPool commandPool,
            VkRenderPass renderPass,
            VkExtent2D extent,
            VkPipeline pipeline,
            VkPipelineLayout pipelineLayout,
            VkDescriptorSet descriptorSet,
            const std::vector<VkFramebuffer>& framebuffers,
            VkBuffer vertexBuffer,
            uint32_t vertexCount,
            VkBuffer indexBuffer,
            uint32_t indexCount
        );



        /*
            销毁Command Buffer。

            注意：

            Command Buffer属于Command Pool。


            通常：

                CommandPool销毁时

                CommandBuffer会自动释放。


            这里可以选择：

                手动Free

                或只清空vector。


        */
        void Shutdown();



        /*
            获取Command Buffer列表。


            DrawFrame时：

                根据当前Swapchain Image Index

                选择对应CommandBuffer提交。


        */
        const std::vector<VkCommandBuffer>& GetCommandBuffers() const
        {
            return m_CommandBuffers;
        }



    private:

        /*
            从Command Pool分配Command Buffer。


            count:

                通常等于Swapchain Image数量。

        */
        void AllocateCommandBuffers(
            VkCommandPool commandPool,
            uint32_t count
        );



        /*
            录制GPU命令。


            包含：

                Begin Command Buffer

                Begin RenderPass

                Bind Pipeline

                Draw

                End RenderPass

                End Command Buffer


        */
        void RecordCommandBuffers(
            VkRenderPass renderPass,
            VkExtent2D extent,
            VkPipeline pipeline,
            VkPipelineLayout pipelineLayout,
            VkDescriptorSet descriptorSet,
            const std::vector<VkFramebuffer>& framebuffers,
            VkBuffer vertexBuffer,
            uint32_t vertexCount,
            VkBuffer indexBuffer,
            uint32_t indexCount
        );



    private:

        /*
            Vulkan Logical Device。

        */
        VkDevice m_Device =
            VK_NULL_HANDLE;

        VkBuffer m_IndexBuffer =
            VK_NULL_HANDLE;


        uint32_t m_IndexCount = 0;

        /*
            Command Buffer集合。

            数量通常：

                = Swapchain Image数量


        */
        std::vector<VkCommandBuffer> m_CommandBuffers;

    };

}