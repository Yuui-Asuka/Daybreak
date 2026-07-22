#include "VulkanBuffer.h"

#include <stdexcept>
#include <cstring>


namespace Daybreak
{


    void VulkanBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage)
    {
        m_Device = device;

        m_PhysicalDevice =
            physicalDevice;


        VkBufferCreateInfo bufferInfo{};

        bufferInfo.sType =
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;


        bufferInfo.size =
            size;


        bufferInfo.usage =
            usage;


        bufferInfo.sharingMode =
            VK_SHARING_MODE_EXCLUSIVE;



        if (vkCreateBuffer(
            m_Device,
            &bufferInfo,
            nullptr,
            &m_Buffer)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create buffer!"
            );
        }



        VkMemoryRequirements requirements{};


        vkGetBufferMemoryRequirements(
            m_Device,
            m_Buffer,
            &requirements
        );



        VkMemoryAllocateInfo allocateInfo{};


        allocateInfo.sType =
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;


        allocateInfo.allocationSize =
            requirements.size;



        allocateInfo.memoryTypeIndex =
            FindMemoryType(
                requirements.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );



        if (vkAllocateMemory(
            m_Device,
            &allocateInfo,
            nullptr,
            &m_Memory)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to allocate buffer memory!"
            );
        }



        vkBindBufferMemory(
            m_Device,
            m_Buffer,
            m_Memory,
            0
        );

    }



    void VulkanBuffer::Upload(
        const void* data,
        VkDeviceSize size)
    {
        void* mapped = nullptr;


        vkMapMemory(
            m_Device,
            m_Memory,
            0,
            size,
            0,
            &mapped
        );


        memcpy(
            mapped,
            data,
            size
        );


        vkUnmapMemory(
            m_Device,
            m_Memory
        );
    }




    uint32_t VulkanBuffer::FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties)
    {

        VkPhysicalDeviceMemoryProperties memoryProperties{};


        vkGetPhysicalDeviceMemoryProperties(
            m_PhysicalDevice,
            &memoryProperties
        );



        for (uint32_t i = 0;
            i < memoryProperties.memoryTypeCount;
            i++)
        {

            if ((typeFilter & (1 << i)) &&
                (memoryProperties.memoryTypes[i].propertyFlags
                    & properties)
                == properties)
            {
                return i;
            }

        }



        throw std::runtime_error(
            "Failed to find suitable memory type!"
        );
    }





    void VulkanBuffer::Shutdown()
    {

        if (m_Buffer != VK_NULL_HANDLE)
        {

            vkDestroyBuffer(
                m_Device,
                m_Buffer,
                nullptr
            );


            m_Buffer =
                VK_NULL_HANDLE;
        }



        if (m_Memory != VK_NULL_HANDLE)
        {

            vkFreeMemory(
                m_Device,
                m_Memory,
                nullptr
            );


            m_Memory =
                VK_NULL_HANDLE;
        }

    }


}