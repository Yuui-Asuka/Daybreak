#include "VulkanBuffer.h"

#include <stdexcept>
#include <iostream>
#include <Renderer/Mesh/Vertex.h>


namespace Daybreak
{

    void VulkanBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        const std::vector<Vertex>& vertices)
    {
        m_Device = device;

        m_PhysicalDevice =
            physicalDevice;


        VkDeviceSize size =
            sizeof(Vertex) *
            vertices.size();


        VkBufferCreateInfo bufferInfo{};

        bufferInfo.sType =
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

        bufferInfo.size =
            size;

        bufferInfo.usage =
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

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


        vkAllocateMemory(
            m_Device,
            &allocateInfo,
            nullptr,
            &m_Memory
        );


        vkBindBufferMemory(
            m_Device,
            m_Buffer,
            m_Memory,
            0
        );


        void* data;


        vkMapMemory(
            m_Device,
            m_Memory,
            0,
            size,
            0,
            &data
        );


        memcpy(
            data,
            vertices.data(),
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

            /*
             判断：

             1.
             这个 Memory Type 是否支持

             2.
             是否满足需要的属性


             例如：

             Vertex Buffer 上传阶段：

             HOST_VISIBLE
             HOST_COHERENT


             GPU最终使用：

             DEVICE_LOCAL

            */

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