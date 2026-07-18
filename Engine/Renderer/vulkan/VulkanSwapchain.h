#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <vector>


namespace Daybreak
{

    /*
        Swapchain支持信息

        Vulkan创建Swapchain之前，需要查询当前GPU和Surface支持哪些能力。

        包括：

        1. Surface能力
           - 最大最小交换图片数量
           - 支持的分辨率范围
           - Transform方式等

        2. Surface格式
           - 图片格式
           - 颜色空间

        3. Present模式
           - FIFO
           - Mailbox
           - Immediate 等

        根据这些信息选择最适合的Swapchain配置。
    */
    struct SwapchainSupportDetails
    {

        /*
            Surface基本能力

            例如：

            minImageCount
                最少需要多少张Swapchain Image

            maxImageCount
                最大允许多少张Swapchain Image

            currentExtent
                当前窗口大小

            currentTransform
                Surface旋转方式
        */
        VkSurfaceCapabilitiesKHR Capabilities;


        /*
            支持的图片格式列表

            例如：

            VK_FORMAT_B8G8R8A8_SRGB

            每个Surface可能支持多个格式，
            创建Swapchain时需要选择一个。
        */
        std::vector<VkSurfaceFormatKHR> Formats;


        /*
            支持的呈现模式

            例如：

            FIFO:
                类似传统VSync

            MAILBOX:
                三缓冲模式

            IMMEDIATE:
                立即显示，可能撕裂
        */
        std::vector<VkPresentModeKHR> PresentModes;

    };


    /*
        VulkanSwapchain

        管理 Vulkan Swapchain 生命周期。

        Swapchain 是 Vulkan 显示系统的核心：

        GPU渲染 ---> Swapchain Image ---> Surface ---> 屏幕


        主要负责：

        1. 创建Swapchain

        2. 获取Swapchain Images

        3. 创建ImageView

        4. 保存图片格式和尺寸信息


        生命周期：

        Init()
            |
            |-- Query支持信息
            |
            |-- 创建VkSwapchainKHR
            |
            |-- 获取VkImage
            |
            |-- 创建VkImageView


        Shutdown()
            |
            |-- 销毁ImageView
            |
            |-- 销毁Swapchain

    */
    class VulkanDevice;


    class VulkanSwapchain
    {

    public:


        /*
            初始化Swapchain

            参数：

            device
                Vulkan逻辑设备

            surface
                GLFW创建的窗口Surface

            window
                GLFW窗口对象

            Swapchain需要同时知道：

            GPU设备
            显示Surface
            窗口尺寸

        */
        void Init(
            VulkanDevice* device,
            VkSurfaceKHR surface,
            GLFWwindow* window
        );



        /*
            获取Swapchain图片格式

            RenderPass创建时需要使用同样的Format。

            例如：

            VK_FORMAT_B8G8R8A8_SRGB
        */
        VkFormat GetImageFormat() const
        {
            return m_ImageFormat;
        }



        /*
            获取Swapchain尺寸

            Pipeline中的Viewport、
            Framebuffer创建都会使用。
        */
        VkExtent2D GetExtent() const
        {
            return m_Extent;
        }



        /*
            获取所有Swapchain ImageView

            Framebuffer创建需要：

            一个Framebuffer对应一个Swapchain ImageView。

            例如：

            Swapchain有3张图片：

            ImageView[0]
            ImageView[1]
            ImageView[2]


            就会创建三个Framebuffer。
        */
        const std::vector<VkImageView>& GetImageViews() const
        {
            return m_ImageViews;
        }



        /*
            获取VkSwapchainKHR句柄

            DrawFrame中：

            vkAcquireNextImageKHR()

            需要使用Swapchain对象。
        */
        VkSwapchainKHR GetSwapchain() const
        {
            return m_Swapchain;
        }



        /*
            获取Swapchain Image数量

            通常：

            双缓冲:
                2

            三缓冲:
                3

            用于创建CommandBuffer数量。
        */
        uint32_t GetImageCount() const
        {
            return static_cast<uint32_t>(
                m_Images.size()
                );
        }



        /*
            销毁Swapchain资源
        */
        void Shutdown();



    private:


        /*
            查询GPU支持的Swapchain能力

            返回：

            - Surface能力
            - Format列表
            - Present模式列表

        */
        SwapchainSupportDetails QuerySwapchainSupport();



        /*
            从支持的Format中选择最佳格式

            通常选择：

            VK_FORMAT_B8G8R8A8_SRGB

            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        */
        VkSurfaceFormatKHR ChooseSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& formats
        );



        /*
            选择Present模式

            优先：

            MAILBOX

            如果不支持：

            FIFO

        */
        VkPresentModeKHR ChoosePresentMode(
            const std::vector<VkPresentModeKHR>& presentModes
        );



        /*
            选择Swapchain分辨率

            通常根据：

            GLFW窗口大小

            Surface限制

            计算最终Extent。
        */
        VkExtent2D ChooseExtent(
            const VkSurfaceCapabilitiesKHR& capabilities
        );



        /*
            创建VkSwapchainKHR

            负责：

            创建交换链对象

            指定：

            - 图片数量
            - 图片格式
            - 分辨率
            - Present模式
        */
        void CreateSwapchain(
            const SwapchainSupportDetails& details
        );



        /*
            获取Swapchain内部的VkImage

            Vulkan创建Swapchain后，

            Image由驱动管理。

            我们只获取句柄。
        */
        void GetSwapchainImages();



        /*
            创建ImageView

            VkImage不能直接用于RenderPass。

            需要：

            VkImage

                |

                v

            VkImageView

                |

                v

            Framebuffer

        */
        void CreateImageViews();



    private:


        /*
            Vulkan逻辑设备

            用于调用：

            vkCreateSwapchainKHR

            vkDestroySwapchainKHR

            vkCreateImageView

        */
        VulkanDevice* m_Device = nullptr;



        /*
            Window Surface

            Swapchain最终显示到这个Surface。
        */
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;



        /*
            Vulkan交换链对象

            GPU和窗口之间的缓冲队列。
        */
        VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;



        /*
            GLFW窗口对象

            用于查询窗口大小。
        */
        GLFWwindow* m_Window = nullptr;



        /*
            Swapchain Image列表

            注意：

            这些Image不是自己创建的。

            Vulkan驱动创建，我们获取引用。
        */
        std::vector<VkImage> m_Images;



        /*
            Image对应的View。

            RenderPass绑定Framebuffer时使用。

        */
        std::vector<VkImageView> m_ImageViews;



        /*
            Swapchain图片格式

            RenderPass附件格式必须一致。
        */
        VkFormat m_ImageFormat =
            VK_FORMAT_UNDEFINED;



        /*
            Swapchain尺寸

            通常等于窗口大小。
        */
        VkExtent2D m_Extent{};

    };

}