#pragma once

#include <vulkan/vulkan.h>

namespace Daybreak
{

    /*
        VulkanCommandPool

        管理 Vulkan Command Pool。


        Command Pool 用于分配 Command Buffer。


        GPU执行流程：

            CPU

             |
             |
             v

        Command Buffer

             |
             |
             v

        Command Pool

             |
             |
             v

            GPU


        Command Buffer 必须从 Command Pool 中创建。


        当前：

            一个 Graphics Queue Family

            一个 Command Pool


    */
    class VulkanCommandPool
    {
    public:

        /*
            初始化Command Pool。


            device:

                Vulkan Logical Device。


            graphicsQueueFamily:

                Graphics Queue所在的Family Index。


                Command Pool创建时需要指定：

                    哪个Queue Family

                后续Command Buffer提交到对应Queue。

        */
        void Init(
            VkDevice device,
            uint32_t graphicsQueueFamily
        );


        /*
            销毁Command Pool。

            Command Pool销毁后：

                所有由它分配的Command Buffer都会失效。

        */
        void Shutdown();



        /*
            获取Command Pool。


            用于：

                vkAllocateCommandBuffers()

        */
        VkCommandPool GetCommandPool() const
        {
            return m_CommandPool;
        }


    private:

        /*
            创建VkCommandPool。

        */
        void CreateCommandPool(
            uint32_t graphicsQueueFamily
        );


    private:

        /*
            Vulkan Logical Device。

            Command Pool属于Device资源。

        */
        VkDevice m_Device =
            VK_NULL_HANDLE;



        /*
            Vulkan Command Pool句柄。

        */
        VkCommandPool m_CommandPool =
            VK_NULL_HANDLE;

    };

}