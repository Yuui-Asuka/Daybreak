#include "VulkanDevice.h"

#include <vector>
#include <iostream>


namespace Daybreak
{

    /**
     * @brief Initializes the Vulkan logical device.
     *
     * Initialization steps:
     *
     * 1. Store Vulkan instance and surface handles.
     * 2. Select a suitable physical device.
     * 3. Create the logical device.
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
     * @param instance Vulkan instance used for device creation.
     * @param surface Window surface used for presentation support.
     */
    void VulkanDevice::Init(
        VkInstance instance,
        VkSurfaceKHR surface)
    {

        std::cout
            << "Init this = "
            << this
            << std::endl;


        // Store Vulkan instance handle.
        m_Instance = instance;


        // Store window surface handle.
        m_Surface = surface;


        // Select the physical GPU.
        PickPhysicalDevice();


        // Create the logical device.
        CreateLogicalDevice();

    }



    /**
     * @brief Selects a suitable physical GPU.
     *
     * A physical device represents an actual GPU available on the system.
     *
     * Examples:
     *
     * - NVIDIA RTX series
     * - AMD Radeon series
     * - Intel integrated graphics
     *
     * The current selection strategy prefers discrete GPUs.
     */
    void VulkanDevice::PickPhysicalDevice()
    {

        uint32_t deviceCount = 0;


        // Query the number of available GPUs.
        vkEnumeratePhysicalDevices(
            m_Instance,
            &deviceCount,
            nullptr
        );


        if (deviceCount == 0)
        {
            throw std::runtime_error(
                "No Vulkan GPU Found!"
            );
        }


        // Allocate storage for physical devices.
        std::vector<VkPhysicalDevice> devices(
            deviceCount
        );


        // Retrieve all available physical devices.
        vkEnumeratePhysicalDevices(
            m_Instance,
            &deviceCount,
            devices.data()
        );


        // Search for a suitable GPU.
        for (auto device : devices)
        {

            VkPhysicalDeviceProperties properties{};


            // Retrieve GPU properties.
            vkGetPhysicalDeviceProperties(
                device,
                &properties
            );


            std::cout
                << "Found GPU: "
                << properties.deviceName
                << std::endl;


            // Select a discrete GPU when available.
            if (properties.deviceType ==
                VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {

                m_PhysicalDevice = device;

                break;

            }

        }


        if (m_PhysicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error(
                "Failed to find suitable GPU"
            );
        }


        VkPhysicalDeviceProperties properties{};


        // Retrieve selected GPU information.
        vkGetPhysicalDeviceProperties(
            m_PhysicalDevice,
            &properties
        );


        std::cout
            << "Selected GPU: "
            << properties.deviceName
            << std::endl;

    }



    /**
     * @brief Creates the Vulkan logical device.
     *
     * The logical device provides an interface for communicating
     * with the selected physical GPU.
     *
     * Creation steps:
     *
     * 1. Query queue families.
     * 2. Find a graphics queue family.
     * 3. Create VkDevice.
     * 4. Retrieve device queues.
     */
    void VulkanDevice::CreateLogicalDevice()
    {

        uint32_t queueFamilyCount = 0;


        // Query available queue families.
        vkGetPhysicalDeviceQueueFamilyProperties(
            m_PhysicalDevice,
            &queueFamilyCount,
            nullptr
        );


        std::vector<VkQueueFamilyProperties> queueFamilies(
            queueFamilyCount
        );


        // Retrieve queue family information.
        vkGetPhysicalDeviceQueueFamilyProperties(
            m_PhysicalDevice,
            &queueFamilyCount,
            queueFamilies.data()
        );


        uint32_t graphicsFamily = UINT32_MAX;


        // Search for a queue family supporting graphics operations.
        for (uint32_t i = 0;
            i < queueFamilyCount;
            i++)
        {

            if (queueFamilies[i].queueFlags &
                VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsFamily = i;
                break;
            }

        }


        if (graphicsFamily == UINT32_MAX)
        {
            throw std::runtime_error(
                "No graphics queue found"
            );
        }


        // Store graphics queue family index.
        m_GraphicsQueueFamily =
            graphicsFamily;


        std::cout
            << "Graphics Queue Family Found: "
            << graphicsFamily
            << std::endl;


        // Define queue priority.
        float queuePriority = 1.0f;


        VkDeviceQueueCreateInfo queueCreateInfo{};


        queueCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;


        // Specify queue family index.
        queueCreateInfo.queueFamilyIndex =
            graphicsFamily;


        // Create one queue from this family.
        queueCreateInfo.queueCount = 1;


        // Assign queue priority.
        queueCreateInfo.pQueuePriorities =
            &queuePriority;

        VkDeviceCreateInfo deviceCreateInfo{};


        deviceCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;


        // Attach queue creation information.
        deviceCreateInfo.queueCreateInfoCount = 1;

        deviceCreateInfo.pQueueCreateInfos =
            &queueCreateInfo;



        // Enable required device extensions.
        const char* deviceExtensions[]
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };


        deviceCreateInfo.enabledExtensionCount =
            1;


        deviceCreateInfo.ppEnabledExtensionNames =
            deviceExtensions;



        /**
         * @brief Creates the Vulkan logical device.
         *
         * After creation, the application can communicate
         * with the GPU through VkDevice and VkQueue objects.
         *
         * GPU
         *  |
         *  v
         * VkDevice
         *  |
         *  v
         * VkQueue
         */
        if (vkCreateDevice(
            m_PhysicalDevice,
            &deviceCreateInfo,
            nullptr,
            &m_Device)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create logical device"
            );
        }



        // Retrieve the graphics queue handle.
        vkGetDeviceQueue(
            m_Device,
            graphicsFamily,
            0,
            &m_GraphicsQueue
        );



        // Retrieve the presentation queue handle.
        vkGetDeviceQueue(
            m_Device,
            graphicsFamily,
            0,
            &m_PresentQueue
        );


        std::cout
            << "Logical Device Created!"
            << std::endl;

    }



    /**
     * @brief Destroys the Vulkan logical device.
     *
     * VkDevice owns many Vulkan resources, including:
     *
     * - Swapchain
     * - Pipeline
     * - CommandPool
     * - Buffer
     * - Image
     *
     * These dependent resources must be destroyed before
     * destroying the logical device.
     */
    void VulkanDevice::Shutdown()
    {

        std::cout
            << "this = "
            << this
            << std::endl;


        std::cout
            << "&m_Device = "
            << &m_Device
            << std::endl;


        std::cout
            << "m_Device = "
            << m_Device
            << std::endl;



        // Destroy the logical device.
        if (m_Device != VK_NULL_HANDLE)
        {

            vkDestroyDevice(
                m_Device,
                nullptr
            );


            // Reset the handle after destruction.
            m_Device = VK_NULL_HANDLE;

        }

    }


}