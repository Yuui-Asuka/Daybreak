#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    class VulkanDescriptorPool
    {

    public:

        void Init(
            VkDevice device
        );


        void Shutdown();


        VkDescriptorPool GetPool() const
        {
            return m_Pool;
        }



    private:

        VkDevice m_Device =
            VK_NULL_HANDLE;


        VkDescriptorPool m_Pool =
            VK_NULL_HANDLE;

    };

}