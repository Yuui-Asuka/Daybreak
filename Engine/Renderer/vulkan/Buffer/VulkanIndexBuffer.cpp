#include "VulkanIndexBuffer.h"

#include <cstring>
#include <stdexcept>


namespace Daybreak
{

    /**
     * @brief Creates and initializes an index buffer.
     *
     * The index buffer stores vertex indices used by indexed drawing.
     *
     * During rendering, the index buffer is bound with:
     *
     * vkCmdBindIndexBuffer()
     *
     * and consumed by:
     *
     * vkCmdDrawIndexed()
     *
     * @param device Vulkan logical device.
     * @param physicalDevice Physical device used for memory allocation.
     * @param indices Index data stored in the buffer.
     */
    void VulkanIndexBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        const std::vector<uint32_t>& indices
    )
    {

        VkDeviceSize size =
            sizeof(uint32_t) * indices.size();


        /**
         * @brief Create underlying Vulkan buffer.
         *
         * The buffer is created with index buffer usage so that it can
         * be bound as an index source during rendering.
         */
        m_Buffer.Init(
            device,
            physicalDevice,
            size,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        );


        /**
         * @brief Upload index data.
         *
         * The current implementation writes directly into mapped GPU
         * memory.
         *
         * For a production renderer, this is usually replaced with:
         *
         * CPU Memory
         *      |
         *      v
         * Staging Buffer
         *      |
         *      v
         * Device Local Buffer
         */
        void* data = nullptr;


        if (vkMapMemory(
            device,
            m_Buffer.GetMemory(),
            0,
            size,
            0,
            &data)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to map index buffer memory!"
            );
        }


        memcpy(
            data,
            indices.data(),
            size
        );


        vkUnmapMemory(
            device,
            m_Buffer.GetMemory()
        );

    }


    /**
     * @brief Releases index buffer resources.
     */
    void VulkanIndexBuffer::Shutdown()
    {
        m_Buffer.Shutdown();
    }

}