#include "VulkanSync.h"

#include <stdexcept>
#include <iostream>


namespace Daybreak
{

    /**
     * @brief Initializes Vulkan synchronization objects.
     *
     * Synchronization objects are device-level resources and must be
     * created using the Vulkan logical device.
     *
     * This creates:
     *
     * - Semaphore for swapchain image acquisition.
     * - Semaphore for render completion.
     * - Fence for CPU-GPU frame synchronization.
     *
     * @param device Vulkan logical device.
     */
    void VulkanSync::Init(
        VkDevice device)
    {
        m_Device = device;

        CreateSyncObjects();
    }



    /**
     * @brief Creates synchronization primitives used during rendering.
     *
     * The synchronization flow:
     *
     * 1. ImageAvailableSemaphore
     *    Signals that a swapchain image is ready for rendering.
     *
     * 2. GPU submits rendering commands.
     *
     * 3. RenderFinishedSemaphore
     *    Signals that rendering has completed.
     *
     * 4. Presentation waits for RenderFinishedSemaphore.
     *
     * The fence allows the CPU to wait until the previous frame has
     * finished execution on the GPU.
     */
    void VulkanSync::CreateSyncObjects()
    {
        /**
         * @brief Semaphore creation information.
         *
         * Vulkan semaphores do not require additional configuration.
         */
        VkSemaphoreCreateInfo semaphoreInfo{};

        semaphoreInfo.sType =
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;



        /**
         * @brief Fence creation information.
         *
         * A fence is used for CPU-side synchronization with GPU work.
         */
        VkFenceCreateInfo fenceInfo{};

        fenceInfo.sType =
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;



        /**
         * @brief Create the fence in signaled state.
         *
         * The first frame can immediately continue without waiting
         * for a previous GPU submission.
         */
        fenceInfo.flags =
            VK_FENCE_CREATE_SIGNALED_BIT;



        /**
         * @brief Creates the image acquisition semaphore.
         *
         * This semaphore is signaled by:
         *
         * vkAcquireNextImageKHR()
         *
         * and waited by the rendering submission.
         */
        if (vkCreateSemaphore(
            m_Device,
            &semaphoreInfo,
            nullptr,
            &m_ImageAvailableSemaphore)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create image available semaphore!"
            );
        }



        /**
         * @brief Creates the rendering completion semaphore.
         *
         * This semaphore is signaled after rendering commands
         * finish execution and is waited by presentation.
         */
        if (vkCreateSemaphore(
            m_Device,
            &semaphoreInfo,
            nullptr,
            &m_RenderFinishedSemaphore)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create render finished semaphore!"
            );
        }



        /**
         * @brief Creates the frame synchronization fence.
         *
         * The CPU waits on this fence before reusing resources
         * from a previous frame.
         */
        if (vkCreateFence(
            m_Device,
            &fenceInfo,
            nullptr,
            &m_InFlightFence)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create fence!"
            );
        }


        std::cout
            << "Sync Objects Created!"
            << std::endl;
    }



    /**
     * @brief Releases synchronization objects.
     *
     * All synchronization objects are owned by the logical device
     * and must be destroyed before destroying VkDevice.
     */
    void VulkanSync::Shutdown()
    {

        if (m_InFlightFence != VK_NULL_HANDLE)
        {
            vkDestroyFence(
                m_Device,
                m_InFlightFence,
                nullptr
            );

            m_InFlightFence =
                VK_NULL_HANDLE;
        }



        if (m_RenderFinishedSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(
                m_Device,
                m_RenderFinishedSemaphore,
                nullptr
            );

            m_RenderFinishedSemaphore =
                VK_NULL_HANDLE;
        }



        if (m_ImageAvailableSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(
                m_Device,
                m_ImageAvailableSemaphore,
                nullptr
            );

            m_ImageAvailableSemaphore =
                VK_NULL_HANDLE;
        }
    }

}