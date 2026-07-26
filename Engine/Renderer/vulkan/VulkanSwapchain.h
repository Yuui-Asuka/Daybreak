#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <vector>


namespace Daybreak
{

    /**
     * @brief Stores the capabilities supported by a Vulkan surface.
     *
     * Before creating a swapchain, Vulkan requires querying the selected
     * physical device and surface to determine the available configurations.
     *
     * The information includes:
     *
     * - Surface capabilities
     * - Supported image formats
     * - Supported presentation modes
     *
     * The swapchain configuration is selected based on these properties.
     */
    struct SwapchainSupportDetails
    {

        /**
         * @brief Basic surface capabilities.
         *
         * Contains information such as:
         *
         * - Minimum and maximum number of swapchain images.
         * - Supported image dimensions.
         * - Surface transform capabilities.
         */
        VkSurfaceCapabilitiesKHR Capabilities;


        /**
         * @brief Supported surface image formats.
         *
         * Defines the available combinations of:
         *
         * - Pixel format.
         * - Color space.
         *
         * A suitable format must be selected when creating the swapchain.
         */
        std::vector<VkSurfaceFormatKHR> Formats;


        /**
         * @brief Supported presentation modes.
         *
         * Examples:
         *
         * - FIFO:
         *   Synchronizes presentation with vertical refresh.
         *
         * - MAILBOX:
         *   Low-latency mode using additional buffering.
         *
         * - IMMEDIATE:
         *   Presents images immediately and may cause tearing.
         */
        std::vector<VkPresentModeKHR> PresentModes;

    };


    class VulkanDevice;


    /**
     * @brief Manages the Vulkan swapchain lifecycle.
     *
     * The swapchain is responsible for presenting rendered images
     * to the window surface.
     *
     * Responsibilities:
     *
     * - Query swapchain support information.
     * - Create and destroy VkSwapchainKHR.
     * - Retrieve swapchain images.
     * - Create image views for rendering.
     * - Store swapchain format and extent information.
     *
     * Lifetime:
     *
     * Init()
     *   |
     *   +-- Query surface capabilities
     *   |
     *   +-- Create swapchain
     *   |
     *   +-- Retrieve images
     *   |
     *   +-- Create image views
     *
     * Shutdown()
     *   |
     *   +-- Destroy image views
     *   |
     *   +-- Destroy swapchain
     */
    class VulkanSwapchain
    {

    public:


        /**
         * @brief Initializes the swapchain.
         *
         * The swapchain depends on:
         *
         * - Vulkan logical device.
         * - Window surface.
         * - Current window size.
         *
         * @param device Vulkan device wrapper.
         * @param surface Vulkan surface associated with the window.
         * @param window GLFW window handle.
         */
        void Init(
            VulkanDevice* device,
            VkSurfaceKHR surface,
            GLFWwindow* window
        );


        /**
         * @brief Returns the swapchain image format.
         *
         * The render pass attachment format must match this value.
         *
         * @return Vulkan image format.
         */
        VkFormat GetImageFormat() const
        {
            return m_ImageFormat;
        }


        /**
         * @brief Returns the swapchain extent.
         *
         * Used when creating:
         *
         * - Viewport.
         * - Framebuffers.
         *
         * @return Swapchain image dimensions.
         */
        VkExtent2D GetExtent() const
        {
            return m_Extent;
        }


        /**
         * @brief Returns all swapchain image views.
         *
         * Each swapchain image view is normally associated with
         * one framebuffer.
         *
         * Example:
         *
         * Triple buffering:
         *
         * ImageView[0] -> Framebuffer[0]
         * ImageView[1] -> Framebuffer[1]
         * ImageView[2] -> Framebuffer[2]
         *
         * @return Collection of image views.
         */
        const std::vector<VkImageView>& GetImageViews() const
        {
            return m_ImageViews;
        }


        /**
         * @brief Returns the native Vulkan swapchain handle.
         *
         * Used during presentation operations such as:
         *
         * vkAcquireNextImageKHR()
         *
         * @return VkSwapchainKHR handle.
         */
        VkSwapchainKHR GetSwapchain() const
        {
            return m_Swapchain;
        }


        /**
         * @brief Returns the number of swapchain images.
         *
         * The image count is usually:
         *
         * - 2 for double buffering.
         * - 3 for triple buffering.
         *
         * @return Number of swapchain images.
         */
        uint32_t GetImageCount() const
        {
            return static_cast<uint32_t>(
                m_Images.size()
                );
        }


        /**
         * @brief Releases all swapchain resources.
         */
        void Shutdown();


    private:


        /**
         * @brief Queries swapchain support information.
         *
         * Retrieves:
         *
         * - Surface capabilities.
         * - Supported formats.
         * - Supported presentation modes.
         *
         * @return Available swapchain support details.
         */
        SwapchainSupportDetails QuerySwapchainSupport();


        /**
         * @brief Selects the preferred surface format.
         *
         * Usually prefers:
         *
         * VK_FORMAT_B8G8R8A8_SRGB
         *
         * with:
         *
         * VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
         *
         * @param formats Available surface formats.
         * @return Selected surface format.
         */
        VkSurfaceFormatKHR ChooseSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& formats
        );


        /**
         * @brief Selects the preferred presentation mode.
         *
         * Preference order:
         *
         * 1. MAILBOX
         * 2. FIFO fallback
         *
         * @param presentModes Available presentation modes.
         * @return Selected presentation mode.
         */
        VkPresentModeKHR ChoosePresentMode(
            const std::vector<VkPresentModeKHR>& presentModes
        );


        /**
         * @brief Determines the swapchain image extent.
         *
         * The final extent depends on:
         *
         * - Current window size.
         * - Surface limitations.
         *
         * @param capabilities Surface capabilities.
         * @return Selected swapchain extent.
         */
        VkExtent2D ChooseExtent(
            const VkSurfaceCapabilitiesKHR& capabilities
        );

        /**
         * @brief Creates the Vulkan swapchain object.
         *
         * Configures:
         *
         * - Image count.
         * - Image format.
         * - Image extent.
         * - Presentation mode.
         * - Surface association.
         *
         * @param details Swapchain support information used for configuration.
         */
        void CreateSwapchain(
            const SwapchainSupportDetails& details
        );


        /**
         * @brief Retrieves swapchain images.
         *
         * Swapchain images are created and owned by the Vulkan
         * implementation. The application only obtains references
         * to these images.
         */
        void GetSwapchainImages();


        /**
         * @brief Creates image views for swapchain images.
         *
         * VkImage objects cannot be directly attached to rendering
         * operations. Image views provide the resource description
         * required by render passes and framebuffers.
         *
         * Image lifecycle:
         *
         * VkImage
         *     |
         *     v
         * VkImageView
         *     |
         *     v
         * Framebuffer
         */
        void CreateImageViews();


    private:


        /**
         * @brief Reference to the Vulkan device wrapper.
         *
         * Used for Vulkan resource creation and destruction:
         *
         * - vkCreateSwapchainKHR()
         * - vkDestroySwapchainKHR()
         * - vkCreateImageView()
         */
        VulkanDevice* m_Device = nullptr;


        /**
         * @brief Window surface associated with this swapchain.
         *
         * The swapchain presents rendered images to this surface.
         */
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;


        /**
         * @brief Vulkan swapchain handle.
         *
         * Represents the queue of images used for presentation
         * between the GPU and the window system.
         */
        VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;


        /**
         * @brief Native GLFW window handle.
         *
         * Used to query the current window size when determining
         * swapchain extent.
         */
        GLFWwindow* m_Window = nullptr;


        /**
         * @brief Swapchain images.
         *
         * These images are created internally by Vulkan when the
         * swapchain is created. The application only stores their
         * handles.
         */
        std::vector<VkImage> m_Images;


        /**
         * @brief Image views corresponding to swapchain images.
         *
         * Used by render passes and framebuffers during rendering.
         */
        std::vector<VkImageView> m_ImageViews;


        /**
         * @brief Selected swapchain image format.
         *
         * The render pass attachment format must match this value.
         */
        VkFormat m_ImageFormat =
            VK_FORMAT_UNDEFINED;


        /**
         * @brief Swapchain image dimensions.
         *
         * Usually matches the current window framebuffer size.
         */
        VkExtent2D m_Extent{};

    };

}