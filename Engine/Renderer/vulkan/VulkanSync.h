#pragma once

#include <vulkan/vulkan.h>

namespace Daybreak
{

    /*
        VulkanSync

        管理 Vulkan 渲染同步对象。

        Vulkan 是异步执行模型：

            CPU
             |
             v
        提交 CommandBuffer
             |
             v
            GPU执行

        CPU不能直接假设GPU已经完成。

        因此需要：

            Semaphore:
                用于GPU之间的同步。

            Fence:
                用于CPU等待GPU完成。
    */
    class VulkanSync
    {
    public:

        /*
            初始化同步对象。

            device:
                Vulkan Logical Device。
        */
        void Init(
            VkDevice device
        );


        /*
            销毁同步对象。
        */
        void Shutdown();


        /*
            获取Image Available Semaphore。

            用于：

                vkAcquireNextImageKHR()

            表示Swapchain Image已经可以使用。
        */
        VkSemaphore GetImageAvailableSemaphore() const
        {
            return m_ImageAvailableSemaphore;
        }


        /*
            获取Render Finished Semaphore。

            用于：

                vkQueuePresentKHR()

            表示GPU已经完成渲染。
        */
        VkSemaphore GetRenderFinishedSemaphore() const
        {
            return m_RenderFinishedSemaphore;
        }


        /*
            获取In Flight Fence。

            用于：

                CPU等待GPU完成当前Frame。

            DrawFrame流程：

                vkWaitForFences()

                    ↓

                vkResetFences()

                    ↓

                vkQueueSubmit()
        */
        VkFence GetInFlightFence() const
        {
            return m_InFlightFence;
        }


    private:

        /*
            创建同步对象：

                ImageAvailable Semaphore

                RenderFinished Semaphore

                InFlight Fence
        */
        void CreateSyncObjects();


    private:

        /*
            Vulkan Logical Device。
        */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /*
            Swapchain Image获取完成信号。

            vkAcquireNextImageKHR()
            会触发该Semaphore。
        */
        VkSemaphore m_ImageAvailableSemaphore =
            VK_NULL_HANDLE;


        /*
            渲染完成信号。

            GPU完成CommandBuffer后触发。

            Present Queue等待该Semaphore。
        */
        VkSemaphore m_RenderFinishedSemaphore =
            VK_NULL_HANDLE;


        /*
            CPU-GPU同步Fence。

            CPU通过它判断GPU是否完成上一帧。
        */
        VkFence m_InFlightFence =
            VK_NULL_HANDLE;

    };

}