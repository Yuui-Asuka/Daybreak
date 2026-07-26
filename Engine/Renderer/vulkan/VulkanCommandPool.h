#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @brief Vulkan command pool wrapper.
     *
     * A command pool manages the allocation and lifetime of
     * Vulkan command buffers.
     *
     * Command pools are associated with a queue family.
     * Command buffers allocated from a command pool can only be
     * submitted to queues belonging to that queue family.
     *
     * Rendering flow:
     *
     * CPU
     *  |
     *  v
     * Command Buffer
     *  |
     *  v
     * Command Queue
     *  |
     *  v
     * GPU
     *
     * The command pool itself only manages command buffer memory
     * and allocation.
     */
    class VulkanCommandPool
    {
    public:

        /**
         * @brief Initializes command pool.
         *
         * @param device Vulkan logical device.
         *
         * @param graphicsQueueFamily Queue family index that supports
         * graphics operations.
         */
        void Init(
            VkDevice device,
            uint32_t graphicsQueueFamily
        );



        /**
         * @brief Destroys command pool.
         *
         * Destroying the command pool automatically releases all
         * command buffers allocated from this pool.
         */
        void Shutdown();



        /**
         * @brief Returns Vulkan command pool handle.
         *
         * Used when allocating command buffers:
         *
         * vkAllocateCommandBuffers()
         *
         * @return VkCommandPool handle.
         */
        VkCommandPool GetCommandPool() const
        {
            return m_CommandPool;
        }



    private:

        /**
         * @brief Creates Vulkan command pool.
         *
         * The queue family index determines which queues can execute
         * command buffers allocated from this pool.
         */
        void CreateCommandPool(
            uint32_t graphicsQueueFamily
        );



    private:

        /**
         * @brief Vulkan logical device.
         *
         * Required for creating and destroying the command pool.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;



        /**
         * @brief Vulkan command pool handle.
         *
         * Used as the source for command buffer allocation.
         */
        VkCommandPool m_CommandPool =
            VK_NULL_HANDLE;

    };

}