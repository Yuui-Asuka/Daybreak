#include "VulkanCommandPool.h"

#include <stdexcept>
#include <iostream>

namespace Daybreak
{

    void VulkanCommandPool::Init(
        VkDevice device,
        uint32_t graphicsQueueFamily)
    {
        /*
            保存Logical Device。

            Command Pool属于Device资源。

            创建和销毁Command Pool都需要VkDevice。
        */
        m_Device = device;


        /*
            创建Command Pool。

            Command Buffer之后会从这里分配。

        */
        CreateCommandPool(
            graphicsQueueFamily
        );
    }



    void VulkanCommandPool::CreateCommandPool(
        uint32_t graphicsQueueFamily)
    {
        /*
            Command Pool创建信息。

            主要指定：

                1. 属于哪个Queue Family

                2. Command Buffer行为


        */
        VkCommandPoolCreateInfo poolInfo{};


        poolInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;



        /*
            指定Command Buffer提交目标。

            当前：

                Graphics Queue Family


            因为Command Buffer里面包含：

                vkCmdDraw()

            所以必须属于Graphics Queue。

        */
        poolInfo.queueFamilyIndex =
            graphicsQueueFamily;



        /*
            Command Pool Flags。


            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT:


                允许单独Reset Command Buffer。


            如果没有这个flag：

                只能Reset整个Command Pool。


            对于每帧重新录制Command Buffer：

                这个选项比较方便。

        */
        poolInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;



        if (vkCreateCommandPool(
            m_Device,
            &poolInfo,
            nullptr,
            &m_CommandPool)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create command pool!"
            );
        }


        std::cout
            << "Command Pool Created!"
            << std::endl;
    }



    void VulkanCommandPool::Shutdown()
    {
        /*
            销毁Command Pool。


            注意：

            Command Pool销毁时：

                所有从它分配的Command Buffer

                会自动失效。


            所以销毁顺序：

                CommandBuffer

                    ↓

                CommandPool

        */

        if (m_CommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(
                m_Device,
                m_CommandPool,
                nullptr
            );


            m_CommandPool =
                VK_NULL_HANDLE;
        }
    }

}