#include "VulkanIndexBuffer.h"



namespace Daybreak
{


    void VulkanIndexBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        const std::vector<uint32_t>& indices
    )
    {

        VkDeviceSize size =
            sizeof(uint32_t)
            *indices.size();



        m_Buffer.Init(
            device,
            physicalDevice,
            size,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        );


        void* data;


        vkMapMemory(
            device,
            m_Buffer.GetMemory(),
            0,
            size,
            0,
            &data
        );


        memcpy(
            data,
            indices.data(),
            size
        );


        vkUnmapMemory(
            device,
            m_Buffer.GetMemory()
        );

    }


    void VulkanIndexBuffer::Shutdown()
    {

        m_Buffer.Shutdown();

    }


}