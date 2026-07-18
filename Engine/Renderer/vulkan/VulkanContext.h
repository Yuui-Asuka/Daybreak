#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


namespace Daybreak
{
    /**
     * @class VulkanContext
     *
     * @brief Vulkan 全局上下文管理类
     *
     * 负责管理 Vulkan 初始化最基础的资源:
     *
     * - Vulkan Instance
     * - Window Surface
     *
     * 生命周期:
     *
     * Init()
     *     |
     *     创建 Vulkan Instance
     *
     * CreateSurface()
     *     |
     *     创建与窗口系统关联的 Vulkan Surface
     *
     * Shutdown()
     *     |
     *     销毁 Vulkan 资源
     *
     * VulkanContext 是整个 Vulkan Renderer 的入口。
     * 后续的 Physical Device、Logical Device、
     * Swapchain 等对象都依赖这里创建的 Instance 和 Surface。
     */
    class VulkanContext
    {
    public:

        /**
         * @brief 初始化 Vulkan Instance
         *
         * 创建 Vulkan 全局实例。
         *
         * 主要工作:
         *
         * 1. 填充 VkApplicationInfo
         * 2. 指定需要的 Layer
         * 3. 指定需要的 Extension
         * 4. 调用 vkCreateInstance()
         *
         * Instance 是 Vulkan 与驱动交互的入口对象。
         */
        void Init();

        /**
        * @brief 创建 Vulkan Window Surface
        *
        * Surface 是 Vulkan 和窗口系统之间的桥梁。
        *
        * Swapchain 会基于 Surface 创建，
        * 用于将 GPU 渲染结果显示到窗口。
        *
        * @param window GLFW 创建的窗口对象
        */
        void CreateSurface(
            GLFWwindow* window
        );

        /**
         * @brief 销毁 Vulkan Context 所管理的资源
         *
         * 销毁:
         *
         * - VkSurfaceKHR
         * - VkInstance
         *
         * 注意:
         * 其他 Vulkan 对象依赖 Context，
         * 所以 Context 通常最后销毁。
         */
        void Shutdown();

        /**
         * @brief 获取 Vulkan Instance
         *
         * @return VkInstance Vulkan 全局实例句柄
         */
        VkInstance GetInstance() const
        {
            return m_Instance;
        }

        /**
         * @brief 获取 Window Surface
         *
         * @return VkSurfaceKHR 当前窗口对应的 Surface
         */
        VkSurfaceKHR GetSurface() const
        {
            return m_Surface;
        }

    private:

        /**
         * Vulkan Instance
         *
         * Vulkan 程序的顶层对象。
         *
         * 负责:
         * - 连接 Vulkan Loader
         * - 查询 GPU
         * - 创建 Device
         */
        VkInstance m_Instance = VK_NULL_HANDLE;

        /**
         * Window Surface
         *
         * 表示 Vulkan 渲染目标窗口。
         *
         * Swapchain 创建时需要使用该对象。
         */
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

    };


}