#include "VulkanCommandBuffer.h"

#include <stdexcept>
#include <iostream>


namespace Daybreak
{

    /**
     * @brief Initializes command buffers.
     *
     * Command buffers store recorded GPU commands that are later
     * submitted to a Vulkan queue for execution.
     *
     * This function:
     *
     * - Allocates command buffers from a command pool.
     * - Records rendering commands for each framebuffer.
     *
     * @param device Vulkan logical device.
     * @param commandPool Command pool used for allocation.
     * @param renderPass Render pass used during rendering.
     * @param extent Rendering area size.
     * @param pipeline Graphics pipeline.
     * @param pipelineLayout Pipeline layout.
     * @param descriptorSet Descriptor set used by shaders.
     * @param framebuffers Framebuffers associated with swapchain images.
     * @param vertexBuffer Vertex buffer used for drawing.
     * @param vertexCount Number of vertices.
     * @param indexBuffer Index buffer used for indexed drawing.
     * @param indexCount Number of indices.
     */
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
        m_Device = device;

        /*
         * Each framebuffer normally corresponds to one swapchain image.
         *
         * A separate command buffer is recorded for each framebuffer
         * so that it can be submitted when that image is acquired.
         */
        AllocateCommandBuffers(
            commandPool,
            static_cast<uint32_t>(framebuffers.size())
        );


        /*
         * Record rendering commands.
         *
         * Recorded commands include:
         *
         * - Beginning the render pass.
         * - Binding graphics state.
         * - Binding vertex/index buffers.
         * - Issuing draw commands.
         * - Ending the render pass.
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


    /**
     * @brief Allocates primary command buffers.
     *
     * Primary command buffers can be directly submitted to a queue.
     * Secondary command buffers can only be executed from primary
     * command buffers.
     *
     * @param commandPool Command pool used for allocation.
     * @param count Number of command buffers to allocate.
     */
    void VulkanCommandBuffer::AllocateCommandBuffers(
        VkCommandPool commandPool,
        uint32_t count)
    {

        m_CommandBuffers.resize(
            count
        );


        VkCommandBufferAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;


        allocInfo.commandPool =
            commandPool;


        allocInfo.level =
            VK_COMMAND_BUFFER_LEVEL_PRIMARY;


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


    /**
     * @brief Records rendering commands into command buffers.
     *
     * Each command buffer records commands targeting one framebuffer.
     *
     * The recorded commands are executed later when the command buffer
     * is submitted to a graphics queue.
     */
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

        for (size_t i = 0;
            i < m_CommandBuffers.size();
            i++)
        {

            /**
             * @brief Begin command buffer recording.
             *
             * All vkCmd* functions called afterwards will append
             * commands into this command buffer.
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

            /**
             * @brief Render pass clear values.
             *
             * The render pass clears:
             *
             * - Color attachment.
             * - Depth attachment.
             *
             * The values must match the attachments defined in the
             * render pass configuration.
             */
            VkClearValue clearValues[2];


            clearValues[0].color =
            {
                {
                    0.1f,
                    0.1f,
                    0.15f,
                    1.0f
                }
            };


            clearValues[1].depthStencil =
            {
                1.0f,
                0
            };


            /**
             * @brief Render pass execution information.
             *
             * Defines:
             *
             * - Render pass to execute.
             * - Target framebuffer.
             * - Rendering area.
             * - Attachment clear values.
             */
            VkRenderPassBeginInfo renderPassInfo{};


            renderPassInfo.sType =
                VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;


            renderPassInfo.renderPass =
                renderPass;


            /*
             * Each command buffer targets a specific framebuffer.
             */
            renderPassInfo.framebuffer =
                framebuffers[i];


            renderPassInfo.renderArea.offset =
            {
                0,
                0
            };


            renderPassInfo.renderArea.extent =
                extent;


            /*
             * Two attachments are cleared:
             *
             * 0 -> Color attachment
             * 1 -> Depth attachment
             */
            renderPassInfo.clearValueCount =
                2;


            renderPassInfo.pClearValues =
                clearValues;


            /**
             * @brief Begins render pass execution.
             *
             * All subsequent rendering commands operate on the
             * framebuffer associated with this render pass.
             */
            vkCmdBeginRenderPass(
                m_CommandBuffers[i],
                &renderPassInfo,
                VK_SUBPASS_CONTENTS_INLINE
            );


            /**
             * @brief Binds the graphics pipeline.
             *
             * The pipeline contains immutable rendering state:
             *
             * - Shader stages.
             * - Rasterization state.
             * - Depth testing.
             * - Color blending.
             * - Vertex input configuration.
             */
            vkCmdBindPipeline(
                m_CommandBuffers[i],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipeline
            );


            /**
             * @brief Binds descriptor resources.
             *
             * Descriptor sets provide shader-accessible resources
             * such as:
             *
             * - Uniform buffers.
             * - Textures.
             * - Samplers.
             */
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


            /**
             * @brief Bind vertex buffer.
             */
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


            /**
             * @brief Bind index buffer.
             *
             * The index buffer defines the order in which vertices
             * are assembled into primitives.
             */
            vkCmdBindIndexBuffer(
                m_CommandBuffers[i],
                indexBuffer,
                0,
                VK_INDEX_TYPE_UINT32
            );


            /**
             * @brief Issue indexed draw command.
             *
             * Executes rendering using the currently bound:
             *
             * - Pipeline.
             * - Descriptor sets.
             * - Vertex buffer.
             * - Index buffer.
             */
            vkCmdDrawIndexed(
                m_CommandBuffers[i],
                indexCount,
                1,
                0,
                0,
                0
            );


            /**
             * @brief Ends render pass execution.
             */
            vkCmdEndRenderPass(
                m_CommandBuffers[i]
            );


            /**
             * @brief Finishes command buffer recording.
             *
             * The command buffer can now be submitted to a Vulkan
             * queue for execution.
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





    /**
     * @brief Releases command buffer references.
     *
     * Command buffers are owned by the Vulkan command pool.
     * Their actual memory is released when the command pool is
     * destroyed.
     */
    void VulkanCommandBuffer::Shutdown()
    {
        m_CommandBuffers.clear();
    }

}