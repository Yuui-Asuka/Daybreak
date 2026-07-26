#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @class VulkanDevice
     *
     * @brief Manages Vulkan GPU device resources.
     *
     * Responsible for managing the connection between Vulkan
     * and the physical GPU.
     *
     * Main responsibilities:
     *
     * - Selecting a physical device.
     * - Creating a logical device.
     * - Retrieving graphics and presentation queues.
     *
     * Vulkan device hierarchy:
     *
     * VkInstance
     *      |
     *      v
     * VkPhysicalDevice
     *      |
     *      v
     * VkDevice
     *      |
     *      v
     * VkQueue
     *
     * Physical Device:
     *     Represents the actual GPU hardware.
     *
     * Logical Device:
     *     Provides an interface for applications to access the GPU.
     *
     * Queue:
     *     A channel used for submitting GPU commands.
     */
    class VulkanDevice
    {

    public:


        /**
         * @brief Initializes the Vulkan device.
         *
         * Initialization steps:
         *
         * 1. Store Vulkan instance and surface handles.
         * 2. Select a suitable physical device.
         * 3. Create the logical device.
         * 4. Retrieve GPU queues.
         *
         * @param instance Vulkan instance used for device initialization.
         * @param surface Window surface used for presentation support.
         */
        void Init(
            VkInstance instance,
            VkSurfaceKHR surface
        );


        /**
         * @brief Releases Vulkan device resources.
         *
         * Destroys the logical device.
         *
         * All resources created from this device, including:
         *
         * - Swapchain
         * - Pipeline
         * - Framebuffer
         * - CommandBuffer
         *
         * must be released before destroying the device.
         */
        void Shutdown();



        /**
         * @brief Retrieves the selected physical device.
         *
         * The physical device represents the actual GPU hardware.
         *
         * @return VkPhysicalDevice Selected GPU handle.
         */
        VkPhysicalDevice GetPhysicalDevice() const
        {
            return m_PhysicalDevice;
        }



        /**
         * @brief Retrieves the Vulkan logical device.
         *
         * The logical device is used to create Vulkan resources:
         *
         * - Pipeline
         * - Buffer
         * - Image
         * - CommandPool
         *
         * @return VkDevice Logical device handle.
         */
        VkDevice GetDevice() const
        {
            return m_Device;
        }



        /**
         * @brief Retrieves the graphics queue.
         *
         * The graphics queue is used for submitting:
         *
         * - CommandBuffer
         * - Draw commands
         * - Render commands
         *
         * @return VkQueue Graphics queue handle.
         */
        VkQueue GetGraphicsQueue() const
        {
            return m_GraphicsQueue;
        }



        /**
         * @brief Retrieves the graphics queue family index.
         *
         * Queue family index is required when creating:
         *
         * - CommandPool
         * - Device queues
         *
         * @return uint32_t Graphics queue family index.
         */
        uint32_t GetGraphicsQueueFamily() const
        {
            return m_GraphicsQueueFamily;
        }



        /**
         * @brief Retrieves the presentation queue.
         *
         * The presentation queue is responsible for presenting
         * swapchain images to the window surface.
         *
         * In many GPUs, the graphics queue and presentation queue
         * can be the same queue.
         *
         * @return VkQueue Presentation queue handle.
         */
        VkQueue GetPresentQueue() const
        {
            return m_PresentQueue;
        }



    private:


        /**
         * @brief Selects a suitable physical GPU.
         *
         * Checks available GPUs and selects a device that supports
         * required Vulkan features and queue capabilities.
         */
        void PickPhysicalDevice();



        /**
         * @brief Creates the Vulkan logical device.
         *
         * Creates:
         *
         * - VkDevice
         * - Graphics queue
         * - Presentation queue
         */
        void CreateLogicalDevice();



    private:


        /**
         * @brief Vulkan instance handle.
         *
         * Used to query available physical devices.
         */
        VkInstance m_Instance = VK_NULL_HANDLE;



        /**
         * @brief Vulkan window surface handle.
         *
         * Used to check presentation support and create swapchains.
         */
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;



        /**
         * @brief Selected physical GPU handle.
         *
         * Represents the actual graphics hardware.
         */
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;



        /**
         * @brief Vulkan logical device handle.
         *
         * Main interface used by the application to access GPU resources.
         */
        VkDevice m_Device = VK_NULL_HANDLE;



        /**
         * @brief Graphics queue handle.
         *
         * Used for executing rendering commands on the GPU.
         */
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;



        /**
         * @brief Presentation queue handle.
         *
         * Used for presenting rendered images to the window.
         */
        VkQueue m_PresentQueue = VK_NULL_HANDLE;



        /**
         * @brief Graphics queue family index.
         *
         * Identifies the queue family supporting
         * VK_QUEUE_GRAPHICS_BIT operations.
         */
        uint32_t m_GraphicsQueueFamily = 0;


    };


}