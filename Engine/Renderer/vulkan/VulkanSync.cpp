#include "VulkanSync.h"

#include <stdexcept>
#include <iostream>

namespace Daybreak
{

    void VulkanSync::Init(
        VkDevice device)
    {
        /*
            保存Logical Device。

            Semaphore和Fence都是Device资源。

            创建和销毁都需要VkDevice。
        */
        m_Device = device;


        /*
            创建同步对象：

                ImageAvailable Semaphore

                RenderFinished Semaphore

                InFlight Fence
        */
        CreateSyncObjects();
    }



    void VulkanSync::CreateSyncObjects()
    {
        /*
            Semaphore创建信息。

            Semaphore没有额外参数。

            只需要指定结构类型。
        */
        VkSemaphoreCreateInfo semaphoreInfo{};

        semaphoreInfo.sType =
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;



        /*
            Fence创建信息。

            Fence用于CPU等待GPU完成。

        */
        VkFenceCreateInfo fenceInfo{};

        fenceInfo.sType =
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;



        /*
            设置Fence初始状态。

            VK_FENCE_CREATE_SIGNALED_BIT:

                创建后立即处于完成状态。


            这样第一帧DrawFrame：

                vkWaitForFences()

            不会无限等待。
        */
        fenceInfo.flags =
            VK_FENCE_CREATE_SIGNALED_BIT;



        /*
            创建Image Available Semaphore。


            用于：

                vkAcquireNextImageKHR()


            表示：

                Swapchain Image已经获取完成。

        */
        if (vkCreateSemaphore(
            m_Device,
            &semaphoreInfo,
            nullptr,
            &m_ImageAvailableSemaphore)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create image available semaphore!"
            );
        }



        /*
            创建Render Finished Semaphore。


            用于：

                vkQueuePresentKHR()


            表示：

                GPU已经完成渲染。

        */
        if (vkCreateSemaphore(
            m_Device,
            &semaphoreInfo,
            nullptr,
            &m_RenderFinishedSemaphore)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create render finished semaphore!"
            );
        }



        /*
            创建In Flight Fence。


            CPU通过Fence判断：

                GPU是否完成上一帧任务。

        */
        if (vkCreateFence(
            m_Device,
            &fenceInfo,
            nullptr,
            &m_InFlightFence)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create fence!"
            );
        }


        std::cout
            << "Sync Objects Created!"
            << std::endl;
    }



    void VulkanSync::Shutdown()
    {
        /*
            销毁顺序：

                Fence

                Semaphore

                Semaphore


            Semaphore和Fence属于Device资源。

            所以必须在vkDestroyDevice之前销毁。
        */


        if (m_InFlightFence != VK_NULL_HANDLE)
        {
            vkDestroyFence(
                m_Device,
                m_InFlightFence,
                nullptr
            );

            m_InFlightFence =
                VK_NULL_HANDLE;
        }



        if (m_RenderFinishedSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(
                m_Device,
                m_RenderFinishedSemaphore,
                nullptr
            );

            m_RenderFinishedSemaphore =
                VK_NULL_HANDLE;
        }



        if (m_ImageAvailableSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(
                m_Device,
                m_ImageAvailableSemaphore,
                nullptr
            );

            m_ImageAvailableSemaphore =
                VK_NULL_HANDLE;
        }
    }

}