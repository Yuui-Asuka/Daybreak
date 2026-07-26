#include "VulkanCommandPool.h"

#include <stdexcept>
#include <iostream>


namespace Daybreak
{

    /**
     * @brief Initializes Vulkan command pool.
     *
     * A command pool manages the allocation and lifetime of
     * command buffers.
     *
     * Command buffers allocated from this pool must be submitted
     * to a queue belonging to the same queue family.
     *
     * @param device Vulkan logical device.
     * @param graphicsQueueFamily Queue family index used for graphics commands.
     */
    void VulkanCommandPool::Init(
        VkDevice device,
        uint32_t graphicsQueueFamily
    )
    {

        m_Device = device;



        /*
         * Create command pool used for allocating command buffers.
         */
        CreateCommandPool(
            graphicsQueueFamily
        );

    }



    /**
     * @brief Creates Vulkan command pool.
     *
     * The command pool defines:
     *
     * - Queue family that command buffers belong to.
     * - Command buffer reset behavior.
     */
    void VulkanCommandPool::CreateCommandPool(
        uint32_t graphicsQueueFamily
    )
    {

        VkCommandPoolCreateInfo poolInfo{};


        poolInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;



        /**
         * @brief Specify the queue family.
         *
         * Command buffers allocated from this pool can only be
         * submitted to queues from this queue family.
         *
         * Graphics commands such as vkCmdDraw require a graphics
         * capable queue family.
         */
        poolInfo.queueFamilyIndex =
            graphicsQueueFamily;



        /**
         * @brief Allow individual command buffer reset.
         *
         * Without this flag:
         *
         * vkResetCommandPool()
         *
         * is required to reset command buffers.
         *
         * With this flag:
         *
         * vkResetCommandBuffer()
         *
         * can reset individual command buffers.
         */
        poolInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;



        if (vkCreateCommandPool(
            m_Device,
            &poolInfo,
            nullptr,
            &m_CommandPool)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create command pool!"
            );
        }


        std::cout
            << "Command Pool Created!"
            << std::endl;

    }



    /**
     * @brief Destroys command pool.
     *
     * Destroying a command pool automatically releases all
     * command buffers allocated from it.
     *
     * The command pool must be destroyed after all command buffers
     * using it are no longer needed.
     */
    void VulkanCommandPool::Shutdown()
    {

        if (m_CommandPool != VK_NULL_HANDLE)
        {

            vkDestroyCommandPool(
                m_Device,
                m_CommandPool,
                nullptr
            );


            m_CommandPool =
                VK_NULL_HANDLE;

        }

    }

}