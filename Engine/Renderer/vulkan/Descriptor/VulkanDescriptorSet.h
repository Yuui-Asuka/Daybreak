#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    class VulkanDescriptorSet
    {

    public:

        void Init(
            VkDevice device,
            VkDescriptorPool descriptorPool,
            VkDescriptorSetLayout layout,
            VkBuffer uniformBuffer,
            VkDeviceSize bufferSize
        );


        void Shutdown();



        VkDescriptorSet GetDescriptorSet() const
        {
            return m_DescriptorSet;
        }



    private:

        VkDevice m_Device =
            VK_NULL_HANDLE;


        VkDescriptorSet m_DescriptorSet =
            VK_NULL_HANDLE;

    };

}