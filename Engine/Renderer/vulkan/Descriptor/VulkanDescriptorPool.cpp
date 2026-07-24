#include "VulkanDescriptorPool.h"

#include <stdexcept>


namespace Daybreak
{


    void VulkanDescriptorPool::Init(
        VkDevice device
    )
    {

        m_Device = device;


        /*
            描述 Pool 中可以分配什么类型的 Descriptor。

            我们现在只有：

            Uniform Buffer

        */

       // VkDescriptorPoolSize poolSize{};


        //poolSize.type =
        //    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;


        ///*
        //    可以创建多少个 DescriptorSet。

        //    现在只有一个 UBO。

        //    所以先设置 1。

        //*/

        //poolSize.descriptorCount = 1;


        VkDescriptorPoolSize poolSizes[2];


        poolSizes[0].type =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

        poolSizes[0].descriptorCount =
            1;



        poolSizes[1].type =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        poolSizes[1].descriptorCount =
            1;



        VkDescriptorPoolCreateInfo poolInfo{};


        poolInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;



        poolInfo.poolSizeCount = 2;


        poolInfo.pPoolSizes =
            poolSizes;



        /*
            最大 DescriptorSet 数量。

        */

        poolInfo.maxSets = 1;



        if (vkCreateDescriptorPool(
            m_Device,
            &poolInfo,
            nullptr,
            &m_Pool)
            != VK_SUCCESS)
        {

            throw std::runtime_error(
                "Failed to create descriptor pool!"
            );

        }

    }



    void VulkanDescriptorPool::Shutdown()
    {

        if (m_Pool != VK_NULL_HANDLE)
        {

            vkDestroyDescriptorPool(
                m_Device,
                m_Pool,
                nullptr
            );


            m_Pool =
                VK_NULL_HANDLE;

        }

    }


}