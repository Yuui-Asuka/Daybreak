#pragma once


#include "VulkanBuffer.h"

#include <vector>


namespace Daybreak
{

    /**
     * @brief Vulkan index buffer abstraction.
     *
     * An index buffer stores vertex indices used for indexed rendering.
     *
     * During rendering, the buffer is bound with:
     *
     * vkCmdBindIndexBuffer()
     *
     * and consumed by:
     *
     * vkCmdDrawIndexed()
     *
     * This class provides a higher-level wrapper around VulkanBuffer
     * and manages index buffer creation and lifetime.
     */
    class VulkanIndexBuffer
    {

    public:

        /**
         * @brief Creates and uploads an index buffer.
         *
         * The buffer stores uint32 indices used to reference vertices
         * in a vertex buffer.
         *
         * @param device Vulkan logical device.
         * @param physicalDevice Physical device used for memory allocation.
         * @param indices Index data to upload.
         */
        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            const std::vector<uint32_t>& indices
        );


        /**
         * @brief Releases index buffer resources.
         */
        void Shutdown();



        /**
         * @brief Returns the underlying Vulkan buffer handle.
         *
         * Used when binding the index buffer during command recording.
         *
         * @return VkBuffer handle.
         */
        VkBuffer GetBuffer() const
        {
            return m_Buffer.GetBuffer();
        }



    private:

        /**
         * @brief Underlying Vulkan buffer implementation.
         *
         * Handles:
         *
         * - VkBuffer creation.
         * - Memory allocation.
         * - Resource destruction.
         */
        VulkanBuffer m_Buffer;


    };

}