#pragma once

#include "VulkanBuffer.h"

#include <vulkan/vulkan.h>


namespace Daybreak
{

    class VulkanUniformBuffer
    {
    public:

        VulkanUniformBuffer() = default;


        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkDeviceSize size
        );


        void Upload(
            void* data,
            VkDeviceSize size
        );


        void Shutdown();



        VkBuffer GetBuffer() const
        {
            return m_Buffer.GetBuffer();
        }



    private:

        VulkanBuffer m_Buffer;

    };

}