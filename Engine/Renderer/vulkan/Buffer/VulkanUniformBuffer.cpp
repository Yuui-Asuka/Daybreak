#include "VulkanUniformBuffer.h"


namespace Daybreak
{


    void VulkanUniformBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size
    )
    {

        m_Buffer.Init(
            device,
            physicalDevice,
            size,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
        );

    }


    void VulkanUniformBuffer::Upload(
        void* data,
        VkDeviceSize size
    )
    {

        m_Buffer.Upload(
            data,
            size
        );

    }


    void VulkanUniformBuffer::Shutdown()
    {

        m_Buffer.Shutdown();

    }


}