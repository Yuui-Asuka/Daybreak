#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    class VulkanDepthBuffer
    {

    public:

        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkExtent2D extent
        );


        void Shutdown();


        VkImageView GetImageView() const
        {
            return m_ImageView;
        }


    private:

        VkDevice m_Device = VK_NULL_HANDLE;

        VkPhysicalDevice m_PhysicalDevice =
            VK_NULL_HANDLE;


        VkImage m_Image =
            VK_NULL_HANDLE;


        VkDeviceMemory m_Memory =
            VK_NULL_HANDLE;


        VkImageView m_ImageView =
            VK_NULL_HANDLE;


    private:

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        );


    };

}