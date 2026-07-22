#include "VulkanDescriptorSet.h"

#include <stdexcept>


namespace Daybreak
{


    void VulkanDescriptorSet::Init(
        VkDevice device,
        VkDescriptorPool descriptorPool,
        VkDescriptorSetLayout layout,
        VkBuffer uniformBuffer,
        VkDeviceSize bufferSize
    )
    {

        m_Device = device;


        /*
            分配 DescriptorSet

            来源:
                DescriptorPool

            模板:
                DescriptorSetLayout

        */


        VkDescriptorSetAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;


        allocInfo.descriptorPool =
            descriptorPool;


        allocInfo.descriptorSetCount = 1;


        allocInfo.pSetLayouts =
            &layout;



        if (vkAllocateDescriptorSets(
            device,
            &allocInfo,
            &m_DescriptorSet)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to allocate descriptor set!"
            );
        }



        /*
            描述 Uniform Buffer

            告诉 Vulkan:

            binding 0
                 |
                 |
                 v
            这个 VkBuffer

        */


        VkDescriptorBufferInfo bufferInfo{};


        bufferInfo.buffer =
            uniformBuffer;


        bufferInfo.offset = 0;


        bufferInfo.range =
            bufferSize;



        /*
            更新 DescriptorSet

            把 Buffer 绑定进去

        */


        VkWriteDescriptorSet descriptorWrite{};


        descriptorWrite.sType =
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;


        descriptorWrite.dstSet =
            m_DescriptorSet;


        descriptorWrite.dstBinding = 0;


        descriptorWrite.dstArrayElement = 0;


        descriptorWrite.descriptorType =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;


        descriptorWrite.descriptorCount = 1;


        descriptorWrite.pBufferInfo =
            &bufferInfo;



        vkUpdateDescriptorSets(
            device,
            1,
            &descriptorWrite,
            0,
            nullptr
        );

    }



    void VulkanDescriptorSet::Shutdown()
    {

        /*
            DescriptorSet 不需要手动销毁。

            它由 DescriptorPool 管理。

        */


        m_DescriptorSet =
            VK_NULL_HANDLE;

    }


}