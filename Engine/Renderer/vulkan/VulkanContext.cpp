#include "VulkanContext.h"

#include <iostream>
#include <stdexcept>
#include <vector>


namespace Daybreak
{

    /**
     * @brief Initializes the Vulkan instance.
     *
     * The Vulkan instance is the root object of a Vulkan application.
     * It connects the application with the Vulkan loader and enables
     * access to physical devices and other Vulkan resources.
     */
    void VulkanContext::Init()
    {
        // Define application information used during instance creation.
        VkApplicationInfo appInfo{};

        appInfo.sType =
            VK_STRUCTURE_TYPE_APPLICATION_INFO;

        appInfo.pApplicationName =
            "Daybreak";

        appInfo.applicationVersion =
            VK_MAKE_VERSION(1, 0, 0);

        appInfo.pEngineName =
            "Daybreak Engine";

        appInfo.engineVersion =
            VK_MAKE_VERSION(1, 0, 1);

        // Request Vulkan API version 1.3.
        appInfo.apiVersion =
            VK_API_VERSION_1_3;


        // Query Vulkan extensions required by GLFW.
        uint32_t glfwExtensionCount = 0;

        const char** glfwExtensions =
            glfwGetRequiredInstanceExtensions(
                &glfwExtensionCount
            );


        // Enable Vulkan validation layers.
        const char* validationLayers[] =
        {
            "VK_LAYER_KHRONOS_validation"
        };


        // Describe Vulkan instance creation parameters.
        VkInstanceCreateInfo createInfo{};

        createInfo.sType =
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        createInfo.pApplicationInfo =
            &appInfo;

        createInfo.enabledLayerCount =
            1;

        createInfo.ppEnabledLayerNames =
            validationLayers;

        createInfo.enabledExtensionCount =
            glfwExtensionCount;

        createInfo.ppEnabledExtensionNames =
            glfwExtensions;


        // Query available Vulkan validation layers.
        uint32_t layerCount = 0;

        vkEnumerateInstanceLayerProperties(
            &layerCount,
            nullptr
        );

        std::vector<VkLayerProperties> layers(layerCount);

        vkEnumerateInstanceLayerProperties(
            &layerCount,
            layers.data()
        );


        // Print available validation layers.
        for (const auto& layer : layers)
        {
            std::cout
                << layer.layerName
                << std::endl;
        }


        // Create the Vulkan instance.
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
     * @brief Creates a Vulkan surface from a GLFW window.
     *
     * A surface represents the connection between Vulkan and
     * the native window system. It is required for swapchain creation.
     *
     * @param window GLFW window used as the presentation target.
     */
    void VulkanContext::CreateSurface(
        GLFWwindow* window
    )
    {

        // Create a Vulkan surface from the GLFW window.
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
     * @brief Releases all Vulkan context resources.
     *
     * Destroys the surface and Vulkan instance.
     *
     * Vulkan objects depending on this context, such as logical devices
     * and swapchains, must be destroyed before calling this function.
     */
    void VulkanContext::Shutdown()
    {

        // Destroy the Vulkan surface.
        if (m_Surface)
        {
            vkDestroySurfaceKHR(
                m_Instance,
                m_Surface,
                nullptr
            );
        }


        // Destroy the Vulkan instance.
        if (m_Instance)
        {
            vkDestroyInstance(
                m_Instance,
                nullptr
            );
        }

    }

}