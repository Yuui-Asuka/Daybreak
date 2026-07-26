#pragma once

#include <vulkan/vulkan.h>

#include <vector>


namespace Daybreak
{

    /**
     * @brief Manages Vulkan command buffers.
     *
     * Command buffers store recorded GPU commands that are submitted
     * to Vulkan queues for execution.
     *
     * Responsibilities:
     *
     * - Allocate command buffers from a command pool.
     * - Record rendering commands.
     * - Provide recorded command buffers for queue submission.
     *
     * Command buffer lifetime is controlled by the Vulkan command pool.
     */
    class VulkanCommandBuffer
    {
    public:

        /**
         * @brief Initializes command buffers.
         *
         * Allocates command buffers and records rendering commands
         * for each framebuffer.
         *
         * @param device Vulkan logical device.
         * @param commandPool Command pool used for allocation.
         * @param renderPass Render pass used during rendering.
         * @param extent Rendering resolution.
         * @param pipeline Graphics pipeline.
         * @param pipelineLayout Pipeline layout.
         * @param descriptorSet Descriptor resources used by shaders.
         * @param framebuffers Target framebuffers.
         * @param vertexBuffer Vertex buffer used for drawing.
         * @param vertexCount Number of vertices.
         * @param indexBuffer Index buffer used for indexed drawing.
         * @param indexCount Number of indices.
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


        /**
         * @brief Releases command buffer references.
         *
         * Actual command buffer memory is released by the Vulkan
         * command pool.
         */
        void Shutdown();


        /**
         * @brief Returns all allocated command buffers.
         *
         * The returned command buffers can be submitted to a Vulkan
         * queue.
         *
         * @return Recorded command buffer collection.
         */
        const std::vector<VkCommandBuffer>& GetCommandBuffers() const
        {
            return m_CommandBuffers;
        }



    private:

        /**
         * @brief Allocates command buffers from a command pool.
         *
         * @param commandPool Source command pool.
         * @param count Number of command buffers to allocate.
         */
        void AllocateCommandBuffers(
            VkCommandPool commandPool,
            uint32_t count
        );


        /**
         * @brief Records rendering commands into command buffers.
         *
         * Records:
         *
         * - Render pass execution.
         * - Pipeline binding.
         * - Descriptor binding.
         * - Vertex/index buffer binding.
         * - Draw commands.
         *
         * @param renderPass Render pass.
         * @param extent Rendering extent.
         * @param pipeline Graphics pipeline.
         * @param pipelineLayout Pipeline layout.
         * @param descriptorSet Descriptor set.
         * @param framebuffers Target framebuffers.
         * @param vertexBuffer Vertex buffer.
         * @param vertexCount Vertex count.
         * @param indexBuffer Index buffer.
         * @param indexCount Index count.
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

        /**
         * @brief Vulkan logical device.
         *
         * Used for command buffer allocation and management.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /**
         * @brief Index buffer reference.
         *
         * Stores the buffer used for indexed rendering.
         *
         * Currently reserved for future command recording logic.
         */
        VkBuffer m_IndexBuffer =
            VK_NULL_HANDLE;


        /**
         * @brief Number of indices used for indexed drawing.
         */
        uint32_t m_IndexCount = 0;


        /**
         * @brief Collection of allocated command buffers.
         *
         * Each command buffer usually corresponds to one framebuffer
         * in the swapchain.
         */
        std::vector<VkCommandBuffer> m_CommandBuffers;

    };

}