#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{


    /**
     * @class VulkanDevice
     *
     * @brief Vulkan GPU Device 管理类
     *
     * 负责管理 Vulkan 与 GPU 之间的连接。
     *
     * 主要职责:
     *
     * - 选择 Physical Device（物理 GPU）
     * - 创建 Logical Device（逻辑设备）
     * - 获取 Graphics Queue
     * - 获取 Present Queue
     *
     *
     * Vulkan 设备层级关系:
     *
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
     *
     * Physical Device:
     *     表示真实 GPU 硬件
     *
     * Logical Device:
     *     应用程序访问 GPU 的接口
     *
     * Queue:
     *     GPU 执行命令的通道
     *
     */
    class VulkanDevice
    {

    public:


        /**
         * @brief 初始化 Vulkan Device
         *
         * 初始化流程:
         *
         * 1. 保存 Instance 和 Surface
         * 2. 选择合适 GPU
         * 3. 创建 Logical Device
         * 4. 获取 GPU Queue
         *
         * @param instance Vulkan Instance
         * @param surface Window Surface
         */
        void Init(
            VkInstance instance,
            VkSurfaceKHR surface
        );


        /**
         * @brief 销毁 Vulkan Device
         *
         * 销毁:
         *
         * - VkDevice
         *
         * 注意:
         *
         * Device 必须在依赖它的资源之后销毁:
         *
         * Swapchain
         * Pipeline
         * Framebuffer
         * CommandBuffer
         *
         * 都需要先释放。
         */
        void Shutdown();



        /**
         * @brief 获取当前选择的 Physical Device
         *
         * Physical Device 对应实际 GPU。
         *
         * 例如:
         *
         * NVIDIA RTX 3060
         * AMD Radeon
         *
         */
        VkPhysicalDevice GetPhysicalDevice() const
        {
            return m_PhysicalDevice;
        }



        /**
         * @brief 获取 Vulkan Logical Device
         *
         * VkDevice 是创建:
         *
         * - Pipeline
         * - Buffer
         * - Image
         * - CommandPool
         *
         * 等 Vulkan 资源的核心对象。
         */
        VkDevice GetDevice() const
        {
            return m_Device;
        }



        /**
         * @brief 获取 Graphics Queue
         *
         * Graphics Queue 用于提交:
         *
         * - CommandBuffer
         * - Draw Command
         * - Render Command
         *
         */
        VkQueue GetGraphicsQueue() const
        {
            return m_GraphicsQueue;
        }



        /**
         * @brief 获取 Graphics Queue Family Index
         *
         * Queue Family 表示 GPU 支持的一类 Queue。
         *
         * 创建:
         *
         * - CommandPool
         * - Queue
         *
         * 时需要该索引。
         */
        uint32_t GetGraphicsQueueFamily() const
        {
            return m_GraphicsQueueFamily;
        }



        /**
         * @brief 获取 Present Queue
         *
         * Present Queue 用于:
         *
         * 将 Swapchain Image 提交到窗口显示。
         *
         * 通常:
         *
         * Graphics Queue
         * 和
         * Present Queue
         *
         * 可以是同一个 Queue。
         */
        VkQueue GetPresentQueue() const
        {
            return m_PresentQueue;
        }



    private:


        /**
         * @brief 查找并选择合适的 GPU
         *
         * 检查:
         *
         * - GPU 是否支持 Vulkan
         * - GPU 类型
         * - Queue Family
         *
         */
        void PickPhysicalDevice();



        /**
         * @brief 创建 Logical Device
         *
         * 创建:
         *
         * - VkDevice
         * - Graphics Queue
         * - Present Queue
         *
         */
        void CreateLogicalDevice();



    private:


        /**
         * Vulkan Instance
         *
         * 来自 VulkanContext。
         *
         * 用于查询 GPU。
         */
        VkInstance m_Instance = VK_NULL_HANDLE;



        /**
         * Window Surface
         *
         * 用于检查 GPU 是否支持窗口显示。
         *
         * Swapchain 创建时也需要。
         */
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;



        /**
         * Physical Device
         *
         * 表示真实 GPU。
         *
         * 例如:
         *
         * NVIDIA RTX 3060 Laptop GPU
         */
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;



        /**
         * Logical Device
         *
         * Vulkan 应用程序主要使用的 GPU 接口。
         */
        VkDevice m_Device = VK_NULL_HANDLE;



        /**
         * Graphics Queue
         *
         * 负责执行图形渲染命令。
         */
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;



        /**
         * Present Queue
         *
         * 负责将渲染结果显示到窗口。
         */
        VkQueue m_PresentQueue = VK_NULL_HANDLE;



        /**
         * Graphics Queue Family Index
         *
         * 指向支持 VK_QUEUE_GRAPHICS_BIT 的 Queue Family。
         */
        uint32_t m_GraphicsQueueFamily = 0;


    };


}