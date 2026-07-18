#include "VulkanDevice.h"

#include <vector>
#include <iostream>


namespace Daybreak
{


    /**
     * @brief 初始化 Vulkan Device
     *
     * 初始化流程:
     *
     * 1. 保存 Vulkan Instance
     * 2. 保存 Window Surface
     * 3. 查找可用 GPU
     * 4. 创建 Logical Device
     *
     *
     * Vulkan 层级:
     *
     * VkInstance
     *      |
     *      ↓
     * VkPhysicalDevice
     *      |
     *      ↓
     * VkDevice
     *      |
     *      ↓
     * VkQueue
     *
     */
    void VulkanDevice::Init(
        VkInstance instance,
        VkSurfaceKHR surface)
    {

        std::cout
            << "Init this = "
            << this
            << std::endl;



        // 保存 Vulkan 全局 Instance
        //
        // 后续需要通过 Instance 查询 GPU。
        m_Instance = instance;



        // 保存窗口 Surface
        //
        // 用于检查 GPU 是否支持显示。
        //
        // 后续 Swapchain 创建也需要。
        m_Surface = surface;



        // 选择物理 GPU
        PickPhysicalDevice();



        // 创建逻辑设备
        CreateLogicalDevice();

    }



    /**
     * @brief 查找并选择 Physical Device
     *
     * Physical Device 对应真实 GPU。
     *
     * 例如:
     *
     * NVIDIA RTX 3060
     * AMD Radeon
     * Intel UHD
     *
     *
     * 当前策略:
     *
     * 优先选择独立显卡。
     *
     */
    void VulkanDevice::PickPhysicalDevice()
    {

        uint32_t deviceCount = 0;



        /*
            第一次调用:

            获取 GPU 数量。

            Vulkan 的枚举 API 通常采用:

            1. 查询数量
            2. 根据数量分配数组
            3. 再次调用获取数据

        */
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



        // 创建 GPU 列表
        std::vector<VkPhysicalDevice> devices(
            deviceCount
        );



        // 获取所有 GPU
        vkEnumeratePhysicalDevices(
            m_Instance,
            &deviceCount,
            devices.data()
        );



        /*
            遍历 GPU

            查看 GPU 属性，
            选择适合当前 Renderer 的设备。
        */
        for (auto device : devices)
        {

            VkPhysicalDeviceProperties properties{};



            // 获取 GPU 信息
            vkGetPhysicalDeviceProperties(
                device,
                &properties
            );



            std::cout
                << "Found GPU: "
                << properties.deviceName
                << std::endl;



            /*
                当前简单策略:

                只选择独立显卡。

                VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:

                NVIDIA / AMD 独立 GPU

                如果没有独显，
                当前代码会失败。
            */
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



        // 输出最终选择的 GPU
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
     * @brief 创建 Logical Device
     *
     * Logical Device 是 Vulkan 应用程序访问 GPU 的接口。
     *
     * 创建流程:
     *
     * 1. 查询 Queue Family
     * 2. 找到 Graphics Queue
     * 3. 创建 VkDevice
     * 4. 获取 Queue
     *
     */
    void VulkanDevice::CreateLogicalDevice()
    {

        uint32_t queueFamilyCount = 0;



        /*
            查询 GPU 支持多少 Queue Family。

            Queue Family 表示 GPU 中
            一类执行任务的 Queue 集合。

            例如:

            Graphics Queue
            Compute Queue
            Transfer Queue

        */
        vkGetPhysicalDeviceQueueFamilyProperties(
            m_PhysicalDevice,
            &queueFamilyCount,
            nullptr
        );



        std::vector<VkQueueFamilyProperties> queueFamilies(
            queueFamilyCount
        );



        vkGetPhysicalDeviceQueueFamilyProperties(
            m_PhysicalDevice,
            &queueFamilyCount,
            queueFamilies.data()
        );



        uint32_t graphicsFamily = UINT32_MAX;



        /*
            查找支持 Graphics 的 Queue Family

            VK_QUEUE_GRAPHICS_BIT:
            表示该 Queue 可以执行:

            - Vertex Shader
            - Fragment Shader
            - Draw Command

        */
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



        // 保存 Queue Family Index
        //
        // CommandPool 创建时需要。
        m_GraphicsQueueFamily =
            graphicsFamily;



        std::cout
            << "Graphics Queue Family Found: "
            << graphicsFamily
            << std::endl;



        /*
            Queue 优先级

            Vulkan 支持一个 Family 创建多个 Queue。

            当前只创建一个 Graphics Queue。
        */
        float queuePriority = 1.0f;



        VkDeviceQueueCreateInfo queueCreateInfo{};



        queueCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;



        // 指定 Queue 所属 Family
        queueCreateInfo.queueFamilyIndex =
            graphicsFamily;



        // 创建 Queue 数量
        queueCreateInfo.queueCount = 1;



        // Queue 优先级
        queueCreateInfo.pQueuePriorities =
            &queuePriority;




        VkDeviceCreateInfo deviceCreateInfo{};



        deviceCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;



        // 添加 Queue 创建信息
        deviceCreateInfo.queueCreateInfoCount = 1;


        deviceCreateInfo.pQueueCreateInfos =
            &queueCreateInfo;




        /*
            Device Extension

            Swapchain 并不是 Vulkan Core 功能。

            必须开启:

            VK_KHR_swapchain

            否则:

            vkCreateSwapchainKHR()

            无法使用。
        */
        const char* deviceExtensions[]
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };



        deviceCreateInfo.enabledExtensionCount =
            1;



        deviceCreateInfo.ppEnabledExtensionNames =
            deviceExtensions;




        /*
            创建 Logical Device

            创建完成后:

            GPU
             |
             ↓
            VkDevice
             |
             ↓
            Queue

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



        /*
            获取 Graphics Queue

            后续提交 CommandBuffer 时使用。
        */
        vkGetDeviceQueue(
            m_Device,
            graphicsFamily,
            0,
            &m_GraphicsQueue
        );



        /*
            获取 Present Queue

            当前 Demo 中:

            Graphics Queue
            =
            Present Queue

            但是实际项目中可能不同。
        */
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
     * @brief 销毁 Vulkan Device
     *
     * VkDevice 是大量 Vulkan 资源的父对象。
     *
     * 依赖关系:
     *
     * Device
     *  |
     *  ├── Swapchain
     *  ├── Pipeline
     *  ├── CommandPool
     *  └── Buffer/Image
     *
     *
     * 所以 Device 必须最后销毁。
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



        if (m_Device != VK_NULL_HANDLE)
        {

            vkDestroyDevice(
                m_Device,
                nullptr
            );


            m_Device = VK_NULL_HANDLE;

        }

    }


}