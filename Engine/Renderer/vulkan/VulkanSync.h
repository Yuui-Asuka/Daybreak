#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @brief Manages Vulkan rendering synchronization objects.
     *
     * Vulkan uses an asynchronous execution model where CPU and GPU
     * operate independently.
     *
     * Synchronization primitives are required to coordinate:
     *
     * - Swapchain image acquisition.
     * - Command buffer execution.
     * - Image presentation.
     *
     * This class manages:
     *
     * - Semaphores for GPU queue synchronization.
     * - Fence for CPU-GPU frame synchronization.
     */
    class VulkanSync
    {
    public:

        /**
         * @brief Initializes synchronization objects.
         *
         * Creates the synchronization resources required for rendering.
         *
         * @param device Vulkan logical device used to create resources.
         */
        void Init(
            VkDevice device
        );


        /**
         * @brief Releases all synchronization resources.
         *
         * Synchronization objects must be destroyed before destroying
         * the Vulkan logical device.
         */
        void Shutdown();


        /**
         * @brief Returns the semaphore signaled after swapchain image acquisition.
         *
         * Used with:
         *
         * vkAcquireNextImageKHR()
         *
         * The rendering submission waits for this semaphore before
         * accessing the acquired swapchain image.
         *
         * @return Image acquisition semaphore.
         */
        VkSemaphore GetImageAvailableSemaphore() const
        {
            return m_ImageAvailableSemaphore;
        }


        /**
         * @brief Returns the semaphore signaled after rendering completes.
         *
         * Used by presentation:
         *
         * vkQueuePresentKHR()
         *
         * The presentation queue waits for this semaphore before
         * displaying the rendered image.
         *
         * @return Render completion semaphore.
         */
        VkSemaphore GetRenderFinishedSemaphore() const
        {
            return m_RenderFinishedSemaphore;
        }


        /**
         * @brief Returns the frame completion fence.
         *
         * The CPU waits on this fence to ensure that the previous
         * frame submission has completed before reusing resources.
         *
         * Typical frame flow:
         *
         * vkWaitForFences()
         *
         *      |
         *
         * vkResetFences()
         *
         *      |
         *
         * vkQueueSubmit()
         *
         * @return Frame synchronization fence.
         */
        VkFence GetInFlightFence() const
        {
            return m_InFlightFence;
        }


    private:

        /**
         * @brief Creates rendering synchronization objects.
         *
         * Creates:
         *
         * - Image available semaphore.
         * - Render finished semaphore.
         * - In-flight fence.
         */
        void CreateSyncObjects();


    private:

        /**
         * @brief Vulkan logical device used for resource management.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /**
         * @brief Signals that a swapchain image has been acquired.
         *
         * The semaphore is signaled by:
         *
         * vkAcquireNextImageKHR()
         *
         * and waited by the graphics queue submission.
         */
        VkSemaphore m_ImageAvailableSemaphore =
            VK_NULL_HANDLE;


        /**
         * @brief Signals completion of rendering commands.
         *
         * The graphics queue signals this semaphore after finishing
         * command buffer execution.
         *
         * The presentation queue waits for this semaphore before
         * presenting the image.
         */
        VkSemaphore m_RenderFinishedSemaphore =
            VK_NULL_HANDLE;


        /**
         * @brief Synchronizes CPU execution with GPU completion.
         *
         * The CPU waits on this fence before submitting another frame
         * that may reuse GPU resources.
         */
        VkFence m_InFlightFence =
            VK_NULL_HANDLE;

    };

}