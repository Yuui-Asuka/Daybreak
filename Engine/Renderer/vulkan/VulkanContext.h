#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


namespace Daybreak
{

    /**
     * @class VulkanContext
     *
     * @brief Manages the global Vulkan context resources.
     *
     * This class is responsible for creating and destroying the
     * fundamental Vulkan objects required by the renderer:
     *
     * - Vulkan Instance
     * - Window Surface
     *
     * The lifecycle is:
     *
     * Init()
     *     |
     *     Creates Vulkan Instance
     *
     * CreateSurface()
     *     |
     *     Creates a Vulkan surface connected to the window system
     *
     * Shutdown()
     *     |
     *     Releases Vulkan resources
     *
     * VulkanContext is the entry point of the Vulkan renderer.
     * Objects such as Physical Device, Logical Device, and Swapchain
     * depend on the Instance and Surface created here.
     */
    class VulkanContext
    {
    public:

        /**
         * @brief Initializes the Vulkan instance.
         *
         * Creates the global Vulkan instance used by the application.
         *
         * The initialization process includes:
         *
         * 1. Configuring application information.
         * 2. Enabling required validation layers.
         * 3. Enabling required Vulkan extensions.
         * 4. Creating the Vulkan instance.
         */
        void Init();


        /**
         * @brief Creates a Vulkan surface from a GLFW window.
         *
         * The surface represents the connection between Vulkan
         * and the native window system.
         *
         * The swapchain uses this surface to present rendered
         * images to the window.
         *
         * @param window GLFW window used as the rendering target.
         */
        void CreateSurface(
            GLFWwindow* window
        );


        /**
         * @brief Releases all resources managed by VulkanContext.
         *
         * Destroys:
         *
         * - VkSurfaceKHR
         * - VkInstance
         *
         * Other Vulkan objects depending on this context must be
         * destroyed before calling this function.
         */
        void Shutdown();


        /**
         * @brief Retrieves the Vulkan instance.
         *
         * @return VkInstance Vulkan instance handle.
         */
        VkInstance GetInstance() const
        {
            return m_Instance;
        }


        /**
         * @brief Retrieves the window surface.
         *
         * @return VkSurfaceKHR Vulkan window surface handle.
         */
        VkSurfaceKHR GetSurface() const
        {
            return m_Surface;
        }


    private:

        /**
         * @brief Vulkan instance handle.
         *
         * The top-level Vulkan object responsible for:
         *
         * - Connecting with the Vulkan loader.
         * - Accessing physical devices.
         * - Creating logical devices.
         */
        VkInstance m_Instance = VK_NULL_HANDLE;


        /**
         * @brief Vulkan window surface handle.
         *
         * Represents the target window used for presentation.
         * Required when creating the swapchain.
         */
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

    };

}