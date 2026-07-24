#include "VulkanCommandBuffer.h"

#include <stdexcept>
#include <iostream>

namespace Daybreak
{
    void VulkanCommandBuffer::Init(
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
        uint32_t indexCount)
    {

        /*
            保存Logical Device。

            Command Buffer属于Device资源。
        */
        m_Device = device;


        /*
            从Command Pool中分配Command Buffer。


            数量：

                = Framebuffer数量

                = Swapchain Image数量


            因为每一张Swapchain Image
            都需要对应一套绘制命令。

        */
        AllocateCommandBuffers(
            commandPool,
            static_cast<uint32_t>(framebuffers.size())
        );

        /*
            录制Command Buffer。


            录制完成以后：

                Command Buffer中已经保存：

                    Begin RenderPass

                    Bind Pipeline

                    Draw Triangle

                    End RenderPass


            GPU提交时直接执行。

        */
        RecordCommandBuffers(
            renderPass,
            extent,
            pipeline,
            pipelineLayout,
            descriptorSet,
            framebuffers,
            vertexBuffer,
            vertexCount,
            indexBuffer,
            indexCount
        );
    }



    void VulkanCommandBuffer::AllocateCommandBuffers(
        VkCommandPool commandPool,
        uint32_t count)
    {
        /*
            创建Command Buffer数组。


            一个Framebuffer对应一个Command Buffer。

        */
        m_CommandBuffers.resize(
            count
        );


        VkCommandBufferAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;


        /*
            Command Buffer来源。

            Command Pool负责管理Command Buffer生命周期。

        */
        allocInfo.commandPool =
            commandPool;



        /*
            Primary Command Buffer。

            Primary:

                可以直接提交给Queue。


            Secondary:

                只能被Primary调用。

        */
        allocInfo.level =
            VK_COMMAND_BUFFER_LEVEL_PRIMARY;



        /*
            分配数量。

        */
        allocInfo.commandBufferCount =
            count;



        if (vkAllocateCommandBuffers(
            m_Device,
            &allocInfo,
            m_CommandBuffers.data())
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to allocate command buffers!"
            );
        }


        std::cout
            << "Allocated "
            << count
            << " Command Buffers"
            << std::endl;
    }



    void VulkanCommandBuffer::RecordCommandBuffers(
        VkRenderPass renderPass,
        VkExtent2D extent,
        VkPipeline pipeline,
        VkPipelineLayout pipelineLayout,
        VkDescriptorSet descriptorSet,
        const std::vector<VkFramebuffer>& framebuffers,
        VkBuffer vertexBuffer,
        uint32_t vertexCount,
        VkBuffer indexBuffer,
        uint32_t indexCount)
    {
        /*
            遍历每一个Command Buffer。

            每个Command Buffer对应一个Framebuffer。

        */
        for (size_t i = 0;
            i < m_CommandBuffers.size();
            i++)
        {

            /*
                开始录制Command Buffer。

                之后所有vkCmdXXX函数都会写入这里。

            */
            VkCommandBufferBeginInfo beginInfo{};


            beginInfo.sType =
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;



            if (vkBeginCommandBuffer(
                m_CommandBuffers[i],
                &beginInfo)
                != VK_SUCCESS)
            {
                throw std::runtime_error(
                    "Failed to begin recording command buffer!"
                );
            }



            /*
                设置Clear颜色。


                RenderPass开始时：

                    清空Color Attachment。


                当前颜色：

                    RGB:

                        0.1
                        0.1
                        0.15


                    Alpha:

                        1.0

            */
            //VkClearValue clearColor{};


            //clearColor.color =
            //{
            //    {
            //        0.1f,
            //        0.1f,
            //        0.15f,
            //        1.0f
            //    }
            //};

            VkClearValue clearValues[2];


            // 颜色
            clearValues[0].color =
            {
                {
                    0.1f,
                    0.1f,
                    0.15f,
                    1.0f
                }
            };


            // 深度
            clearValues[1].depthStencil =
            {
                1.0f,
                0
            };

            /*
                RenderPass开始信息。


                包含：

                    使用哪个RenderPass

                    使用哪个Framebuffer

                    清屏颜色

                    渲染区域

            */
            VkRenderPassBeginInfo renderPassInfo{};


            renderPassInfo.sType =
                VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;



            renderPassInfo.renderPass =
                renderPass;



            /*
                当前Command Buffer对应的Framebuffer。

            */
            renderPassInfo.framebuffer =
                framebuffers[i];



            /*
                渲染区域。

            */
            renderPassInfo.renderArea.offset =
            {
                0,
                0
            };


            renderPassInfo.renderArea.extent =
                extent;



            /*
                Clear Value数量。

                当前只有Color Attachment。

            */
            renderPassInfo.clearValueCount =
                2;


            renderPassInfo.pClearValues =
                clearValues;



            /*
                开始RenderPass。

                后面的Draw命令都会作用于这个Framebuffer。

            */
            vkCmdBeginRenderPass(
                m_CommandBuffers[i],
                &renderPassInfo,
                VK_SUBPASS_CONTENTS_INLINE
            );



            /*
                绑定Graphics Pipeline。


                Pipeline包含：

                    Shader

                    Rasterizer

                    Viewport

                    Blend

                    等状态。


            */
            vkCmdBindPipeline(
                m_CommandBuffers[i],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipeline
            );

            vkCmdBindDescriptorSets(
                m_CommandBuffers[i],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout,
                0,
                1,
                &descriptorSet,
                0,
                nullptr
            );

            VkBuffer vertexBuffers[] =
            {
                vertexBuffer
            };

            VkDeviceSize offsets[] =
            {
                0
            };

            vkCmdBindVertexBuffers(
                m_CommandBuffers[i],
                0,
                1,
                vertexBuffers,
                offsets
            );

            vkCmdBindIndexBuffer(
                m_CommandBuffers[i],
                indexBuffer,
                0,
                VK_INDEX_TYPE_UINT32
            );

            std::cout << "Pipeline      = " << pipeline << std::endl;
            std::cout << "PipelineLayout= " << pipelineLayout << std::endl;
            std::cout << "DescriptorSet = " << descriptorSet << std::endl;
            std::cout << "VertexBuffer  = " << vertexBuffer << std::endl;
            std::cout << "IndexBuffer   = " << indexBuffer << std::endl;
            std::cout << "IndexCount    = " << indexCount << std::endl;

            vkCmdDrawIndexed(
                m_CommandBuffers[i],
                indexCount,
                1,
                0,
                0,
                0
            );

            /*
                结束RenderPass。

            */
            vkCmdEndRenderPass(
                m_CommandBuffers[i]
            );



            /*
                结束Command Buffer录制。

                此后可以提交给Queue执行。

            */
            if (vkEndCommandBuffer(
                m_CommandBuffers[i])
                != VK_SUCCESS)
            {
                throw std::runtime_error(
                    "Failed to record command buffer!"
                );
            }
        }


        std::cout
            << "Command Buffers Recorded!"
            << std::endl;
    }



    void VulkanCommandBuffer::Shutdown()
    {
        /*
            Command Buffer由Command Pool管理。


            真正释放：

                vkDestroyCommandPool()


            所以这里：

                只清空CPU侧vector。

        */
        m_CommandBuffers.clear();
    }

}