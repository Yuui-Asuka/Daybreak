#include "VulkanDevice.h"

#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include "VulkanSwapchain.h"

#include <algorithm>


namespace Daybreak
{


    /*
        初始化Swapchain

        Swapchain创建流程：

        1. 保存Device、Surface、Window

        2. 查询GPU支持能力

        3. 根据支持情况选择：

            - Image Format
            - Present Mode
            - Extent

        4. 创建Swapchain

        5. 获取Swapchain Images

        6. 创建Image Views

    */
    void VulkanSwapchain::Init(
        VulkanDevice* device,
        VkSurfaceKHR surface,
        GLFWwindow* window)
    {

        m_Device = device;

        m_Surface = surface;

        m_Window = window;


        /*
            查询当前GPU对于当前窗口Surface的支持情况

            包括：

            - 支持的图片格式
            - 支持的刷新模式
            - 最大图片数量
            - 图片尺寸限制

        */
        SwapchainSupportDetails details =
            QuerySwapchainSupport();



        CreateSwapchain(details);
    }




    /*
        选择Swapchain图片格式


        Vulkan支持很多颜色格式：

        例如：

        VK_FORMAT_B8G8R8A8_SRGB

        是Windows窗口常用格式。


        如果找到最佳格式则返回。

        否则使用GPU提供的第一个格式。

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





    /*
        选择Present模式


        Present Mode决定GPU如何把图片提交到屏幕。


        MAILBOX:

            类似三缓冲

            优点：
                延迟低
                不容易撕裂


        FIFO:

            类似VSync

            所有GPU必须支持

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





    /*
        选择Swapchain尺寸


        有些Surface会直接指定尺寸：

            currentExtent != UINT32_MAX


        如果没有指定：

            根据窗口大小计算。


        最后使用clamp限制在GPU支持范围内。

    */
    VkExtent2D VulkanSwapchain::ChooseExtent(
        const VkSurfaceCapabilitiesKHR& capabilities)
    {

        if (capabilities.currentExtent.width
            != UINT32_MAX)
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






    /*
        查询Swapchain支持信息


        Vulkan创建Swapchain之前：

        必须知道GPU支持什么。


        查询三部分：


        1.
        Surface能力

        2.
        图片格式

        3.
        Present模式

    */
    SwapchainSupportDetails
        VulkanSwapchain::QuerySwapchainSupport()
    {

        SwapchainSupportDetails details;



        /*
            查询Surface能力

        */
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(

            m_Device->GetPhysicalDevice(),

            m_Surface,

            &details.Capabilities

        );





        /*
            查询支持格式数量

        */
        uint32_t formatCount = 0;


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







        /*
            查询Present模式

        */

        uint32_t presentModeCount = 0;


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






    /*
        创建Swapchain


        Swapchain就是：

        GPU渲染结果暂存的图片队列。



        例如：

        三缓冲：

        Image0

        Image1

        Image2


        GPU渲染其中一张，

        另外两张等待显示。

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





        /*
            创建多少张Swapchain Image

            通常：

            min + 1

            可以减少GPU等待。

        */
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



        /*
            Render目标

            COLOR_ATTACHMENT表示：

            这些Image会作为颜色输出。

        */
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

            &m_Swapchain

        ) != VK_SUCCESS)
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






    /*
        获取Swapchain中的Image


        注意：

        Image不是我们创建。

        是Swapchain内部创建。

        我们只是获取句柄。

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






    /*
        创建ImageView


        Image:

            GPU实际图片


        ImageView:

            告诉Vulkan如何访问这个Image



        RenderPass绑定的是ImageView。

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

                &m_ImageViews[i]

            ) != VK_SUCCESS)
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






    /*
        销毁Swapchain


        注意：

        Image不需要销毁。

        因为它属于Swapchain。


        但是ImageView需要自己销毁。

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