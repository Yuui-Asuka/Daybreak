#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <vector>
#include <iostream>
#include <algorithm>

#include <GLFW/glfw3.h>


namespace Daybreak
{

    /**
     * @brief Initializes the Vulkan swapchain.
     *
     * The initialization process:
     *
     * 1. Store device, surface and window references.
     * 2. Query GPU swapchain capabilities.
     * 3. Select image format, present mode and extent.
     * 4. Create swapchain.
     * 5. Retrieve swapchain images.
     * 6. Create image views.
     *
     * @param device Vulkan device manager used to access GPU resources.
     * @param surface Window surface used by the swapchain.
     * @param window GLFW window used to query framebuffer size.
     */
    void VulkanSwapchain::Init(
        VulkanDevice* device,
        VkSurfaceKHR surface,
        GLFWwindow* window)
    {

        m_Device = device;

        m_Surface = surface;

        m_Window = window;

        SwapchainSupportDetails details =
            QuerySwapchainSupport();

        CreateSwapchain(details);

    }


    /**
     * @brief Selects the preferred surface format.
     *
     * Prefers VK_FORMAT_B8G8R8A8_SRGB with
     * SRGB nonlinear color space.
     *
     * @param formats Available surface formats.
     *
     * @return VkSurfaceFormatKHR Selected surface format.
     */
    VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& formats)
    {
        for (const auto& format : formats)
        {
            if (format.format ==
                VK_FORMAT_B8G8R8A8_SRGB &&

                format.colorSpace ==
                VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return format;
            }
        }
        return formats[0];
    }


    /**
     * @brief Selects the preferred presentation mode.
     *
     * Uses MAILBOX mode when available for lower latency.
     * Falls back to FIFO mode which is guaranteed to exist.
     *
     * @param presentModes Available presentation modes.
     *
     * @return VkPresentModeKHR Selected presentation mode.
     */
    VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(
        const std::vector<VkPresentModeKHR>& presentModes)
    {
        for (const auto& mode : presentModes)
        {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return mode;
            }
        }


        return VK_PRESENT_MODE_FIFO_KHR;
    }


    /**
     * @brief Calculates the swapchain image extent.
     *
     * Uses the surface-provided extent when available.
     * Otherwise calculates the extent from the window size
     * and clamps it to GPU supported limits.
     *
     * @param capabilities Surface capabilities provided by the GPU.
     *
     * @return VkExtent2D Selected swapchain image size.
     */
    VkExtent2D VulkanSwapchain::ChooseExtent(
        const VkSurfaceCapabilitiesKHR& capabilities)
    {

        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }



        int width;
        int height;



        glfwGetFramebufferSize(
            m_Window,
            &width,
            &height
        );



        VkExtent2D extent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };



        extent.width =
            std::clamp(
                extent.width,
                capabilities.minImageExtent.width,
                capabilities.maxImageExtent.width
            );



        extent.height =
            std::clamp(
                extent.height,
                capabilities.minImageExtent.height,
                capabilities.maxImageExtent.height
            );


        return extent;
    }



    /**
     * @brief Queries GPU swapchain support information.
     *
     * Retrieves:
     *
     * - Surface capabilities
     * - Supported image formats
     * - Supported presentation modes
     *
     * @return SwapchainSupportDetails GPU swapchain capabilities.
     */
    SwapchainSupportDetails
        VulkanSwapchain::QuerySwapchainSupport()
    {

        SwapchainSupportDetails details;



        // Query surface capabilities.
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            m_Device->GetPhysicalDevice(),
            m_Surface,
            &details.Capabilities
        );



        uint32_t formatCount = 0;



        // Query supported surface formats.
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            m_Device->GetPhysicalDevice(),
            m_Surface,
            &formatCount,
            nullptr
        );



        if (formatCount > 0)
        {

            details.Formats.resize(
                formatCount
            );


            vkGetPhysicalDeviceSurfaceFormatsKHR(
                m_Device->GetPhysicalDevice(),
                m_Surface,
                &formatCount,
                details.Formats.data()
            );

        }



        uint32_t presentModeCount = 0;



        // Query supported presentation modes.
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            m_Device->GetPhysicalDevice(),
            m_Surface,
            &presentModeCount,
            nullptr
        );



        if (presentModeCount > 0)
        {

            details.PresentModes.resize(
                presentModeCount
            );


            vkGetPhysicalDeviceSurfacePresentModesKHR(
                m_Device->GetPhysicalDevice(),
                m_Surface,
                &presentModeCount,
                details.PresentModes.data()
            );

        }


        return details;

    }



    /**
     * @brief Creates the Vulkan swapchain.
     *
     * The swapchain manages a collection of images used
     * for rendering and presenting frames to the screen.
     *
     * @param details Swapchain capability information.
     */
    void VulkanSwapchain::CreateSwapchain(
        const SwapchainSupportDetails& details)
    {

        VkSurfaceFormatKHR surfaceFormat =
            ChooseSurfaceFormat(
                details.Formats
            );



        VkPresentModeKHR presentMode =
            ChoosePresentMode(
                details.PresentModes
            );



        VkExtent2D extent =
            ChooseExtent(
                details.Capabilities
            );



        m_ImageFormat =
            surfaceFormat.format;


        m_Extent =
            extent;



        uint32_t imageCount =
            details.Capabilities.minImageCount + 1;



        if (details.Capabilities.maxImageCount > 0 &&
            imageCount > details.Capabilities.maxImageCount)
        {
            imageCount =
                details.Capabilities.maxImageCount;
        }



        VkSwapchainCreateInfoKHR createInfo{};


        createInfo.sType =
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;


        createInfo.surface =
            m_Surface;


        createInfo.minImageCount =
            imageCount;


        createInfo.imageFormat =
            surfaceFormat.format;


        createInfo.imageColorSpace =
            surfaceFormat.colorSpace;


        createInfo.imageExtent =
            extent;


        createInfo.imageArrayLayers =
            1;


        createInfo.imageUsage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


        createInfo.imageSharingMode =
            VK_SHARING_MODE_EXCLUSIVE;


        createInfo.preTransform =
            details.Capabilities.currentTransform;


        createInfo.compositeAlpha =
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;


        createInfo.presentMode =
            presentMode;

        createInfo.clipped =
            VK_TRUE;

        createInfo.oldSwapchain =
            VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(
            m_Device->GetDevice(),
            &createInfo,
            nullptr,
            &m_Swapchain)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create Swapchain!"
            );
        }

        std::cout
            << "Swapchain Created!"
            << std::endl;

        GetSwapchainImages();

        CreateImageViews();

    }


    /**
     * @brief Retrieves swapchain images.
     *
     * Swapchain owns the images.
     * This function only retrieves their handles.
     */
    void VulkanSwapchain::GetSwapchainImages()
    {

        uint32_t imageCount = 0;



        vkGetSwapchainImagesKHR(
            m_Device->GetDevice(),
            m_Swapchain,
            &imageCount,
            nullptr
        );



        m_Images.resize(
            imageCount
        );



        vkGetSwapchainImagesKHR(
            m_Device->GetDevice(),
            m_Swapchain,
            &imageCount,
            m_Images.data()
        );



        std::cout
            << "Swapchain Image Count: "
            << imageCount
            << std::endl;

    }







    /**
     * @brief Creates image views for swapchain images.
     *
     * Image views describe how Vulkan accesses images.
     * Render passes use image views as rendering targets.
     */
    void VulkanSwapchain::CreateImageViews()
    {

        m_ImageViews.resize(
            m_Images.size()
        );



        for (size_t i = 0;
            i < m_Images.size();
            i++)
        {

            VkImageViewCreateInfo createInfo{};


            createInfo.sType =
                VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;


            createInfo.image =
                m_Images[i];


            createInfo.viewType =
                VK_IMAGE_VIEW_TYPE_2D;


            createInfo.format =
                m_ImageFormat;


            createInfo.subresourceRange.aspectMask =
                VK_IMAGE_ASPECT_COLOR_BIT;


            createInfo.subresourceRange.levelCount =
                1;


            createInfo.subresourceRange.layerCount =
                1;



            if (vkCreateImageView(
                m_Device->GetDevice(),
                &createInfo,
                nullptr,
                &m_ImageViews[i])
                != VK_SUCCESS)
            {
                throw std::runtime_error(
                    "Failed to create image view!"
                );
            }

        }


        std::cout
            << "Image Views Created!"
            << std::endl;

    }







    /**
     * @brief Releases swapchain resources.
     *
     * Destroys image views and swapchain object.
     *
     * Swapchain images are managed by Vulkan and do not
     * need to be destroyed manually.
     */
    void VulkanSwapchain::Shutdown()
    {

        for (auto imageView : m_ImageViews)
        {

            vkDestroyImageView(
                m_Device->GetDevice(),
                imageView,
                nullptr
            );

        }


        m_ImageViews.clear();



        if (m_Swapchain != VK_NULL_HANDLE)
        {

            vkDestroySwapchainKHR(
                m_Device->GetDevice(),
                m_Swapchain,
                nullptr
            );


            m_Swapchain =
                VK_NULL_HANDLE;

        }

    }


}