#include "VulkanContext.h"

#include <iostream>
#include <stdexcept>


namespace Daybreak
{

    /**
     * @brief 初始化 Vulkan Instance
     *
     * Vulkan Instance 是整个 Vulkan 程序的入口对象。
     *
     * 它负责:
     *
     * - 连接 Vulkan Loader
     * - 查询系统 GPU
     * - 创建后续 Vulkan 对象
     *
     */
    void VulkanContext::Init()
    {
        /*
        应用程序信息

        VkApplicationInfo 不会创建任何 Vulkan 对象，
        它只是告诉 Vulkan 当前程序的信息。

        例如:
        - 应用名称
        - 引擎名称
        - Vulkan API 版本
        */
        VkApplicationInfo appInfo{};

        appInfo.sType =
            VK_STRUCTURE_TYPE_APPLICATION_INFO;

        // 当前应用名称
        appInfo.pApplicationName =
            "Daybreak";

        // 应用版本
        appInfo.applicationVersion =
            VK_MAKE_VERSION(1, 0, 0);

        // 引擎名称
        appInfo.pEngineName =
            "Daybreak Engine";

        // 引擎版本
        appInfo.engineVersion =
            VK_MAKE_VERSION(1, 0, 0);

        /*
        指定 Vulkan API 版本

        当前使用 Vulkan 1.3。
        */
        appInfo.apiVersion =
            VK_API_VERSION_1_3;

        /*
        Vulkan Instance Extension

        Vulkan 默认不知道窗口系统如何创建。

        GLFW 会根据平台返回需要的 Extension:

        Windows:
            VK_KHR_surface
            VK_KHR_win32_surface

        Linux:
            VK_KHR_xcb_surface
            等

        这些 Extension 用于后续创建 Surface。
        */
        uint32_t glfwExtensionCount = 0;

        const char** glfwExtensions =
            glfwGetRequiredInstanceExtensions(
                &glfwExtensionCount
            );

        /*
        Instance 创建信息

        VkInstanceCreateInfo 描述:
        - 使用哪个 ApplicationInfo
        - 开启哪些 Extension
        - 开启哪些 Layer
        */
        VkInstanceCreateInfo createInfo{};

        createInfo.sType =
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        createInfo.pApplicationInfo =
            &appInfo;

        // 开启 GLFW 所需的 Vulkan Extension
        createInfo.enabledExtensionCount =
            glfwExtensionCount;

        createInfo.ppEnabledExtensionNames =
            glfwExtensions;


        /*
        创建 Vulkan Instance

        Instance 是 Vulkan 中最高层对象。

        Physical Device
        Logical Device
        Surface

        都依赖 Instance。
        */
        if (
            vkCreateInstance(
                &createInfo,
                nullptr,
                &m_Instance
            )
            != VK_SUCCESS
            )
        {
            throw std::runtime_error(
                "Failed to create Vulkan Instance"
            );
        }


        std::cout
            << "Vulkan Instance Created!"
            << std::endl;

    }

    /**
     * @brief 创建 Vulkan Surface
     *
     * Surface 是 Vulkan 和窗口系统之间的桥梁。
     *
     * Vulkan 本身不知道:
     *
     * - HWND
     * - X11 Window
     * - macOS Window
     *
     * 所以需要通过 Surface 抽象窗口。
     *
     * 创建流程:
     *
     * GLFW Window
     *       |
     *       ↓
     * VkSurfaceKHR
     *       |
     *       ↓
     * Swapchain
     *
     *
     * @param window GLFW 窗口指针
     */
    void VulkanContext::CreateSurface(
        GLFWwindow* window
    )
    {

        /*
        创建 Vulkan Instance

        Instance 是 Vulkan 中最高层对象。

        Physical Device
        Logical Device
        Surface

        都依赖 Instance。
        */
        if (
            glfwCreateWindowSurface(
                m_Instance,
                window,
                nullptr,
                &m_Surface
            )
            != VK_SUCCESS
            )
        {
            throw std::runtime_error(
                "Failed to create Vulkan Surface"
            );
        }


        std::cout
            << "Vulkan Surface Created"
            << std::endl;

    }

    /**
     * @brief 销毁 Vulkan Context
     *
     * 销毁顺序:
     *
     * Surface
     *    ↓
     * Instance
     *
     *
     * 原因:
     *
     * Surface 创建于 Instance，
     * 所以必须先销毁 Surface。
     *
     * 注意:
     *
     * Device
     * Swapchain
     * Pipeline
     * CommandBuffer
     *
     * 都必须在 Context 前销毁。
     */
    void VulkanContext::Shutdown()
    {

        /*
        销毁窗口 Surface

        Surface 不包含 GPU 资源，
        但是依赖 Vulkan Instance。
        */
        if (m_Surface)
        {
            vkDestroySurfaceKHR(
                m_Instance,
                m_Surface,
                nullptr
            );
        }

        /*
        销毁 Vulkan Instance

        Instance 是 Vulkan 顶层对象。
        */
        if (m_Instance)
        {
            vkDestroyInstance(
                m_Instance,
                nullptr
            );
        }

    }
}