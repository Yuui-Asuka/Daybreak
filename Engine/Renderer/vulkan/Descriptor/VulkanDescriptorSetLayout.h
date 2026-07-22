#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    class VulkanDescriptorSetLayout
    {

    public:

        void Init(
            VkDevice device
        );


        void Shutdown();



        VkDescriptorSetLayout GetLayout() const
        {
            return m_Layout;
        }



    private:

        VkDevice m_Device =
            VK_NULL_HANDLE;


        VkDescriptorSetLayout m_Layout =
            VK_NULL_HANDLE;

    };

}