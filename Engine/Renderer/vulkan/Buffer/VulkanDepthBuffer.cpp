#include "VulkanDepthBuffer.h"

#include <stdexcept>


namespace Daybreak
{


    void VulkanDepthBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkExtent2D extent
    )
    {

        m_Device = device;

        m_PhysicalDevice = physicalDevice;



        VkFormat depthFormat =
            VK_FORMAT_D32_SFLOAT;



        VkImageCreateInfo imageInfo{};


        imageInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;


        imageInfo.imageType =
            VK_IMAGE_TYPE_2D;


        imageInfo.extent.width =
            extent.width;


        imageInfo.extent.height =
            extent.height;


        imageInfo.extent.depth =
            1;


        imageInfo.mipLevels =
            1;


        imageInfo.arrayLayers =
            1;


        imageInfo.format =
            depthFormat;


        imageInfo.tiling =
            VK_IMAGE_TILING_OPTIMAL;


        imageInfo.initialLayout =
            VK_IMAGE_LAYOUT_UNDEFINED;


        imageInfo.usage =
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;


        imageInfo.samples =
            VK_SAMPLE_COUNT_1_BIT;


        imageInfo.sharingMode =
            VK_SHARING_MODE_EXCLUSIVE;



        if (vkCreateImage(
            m_Device,
            &imageInfo,
            nullptr,
            &m_Image)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create depth image!"
            );
        }



        VkMemoryRequirements requirements{};


        vkGetImageMemoryRequirements(
            m_Device,
            m_Image,
            &requirements
        );



        VkMemoryAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;


        allocInfo.allocationSize =
            requirements.size;


        allocInfo.memoryTypeIndex =
            FindMemoryType(
                requirements.memoryTypeBits,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );



        if (vkAllocateMemory(
            m_Device,
            &allocInfo,
            nullptr,
            &m_Memory)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to allocate depth memory!"
            );
        }



        vkBindImageMemory(
            m_Device,
            m_Image,
            m_Memory,
            0
        );



        VkImageViewCreateInfo viewInfo{};


        viewInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;


        viewInfo.image =
            m_Image;


        viewInfo.viewType =
            VK_IMAGE_VIEW_TYPE_2D;


        viewInfo.format =
            depthFormat;


        viewInfo.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_DEPTH_BIT;


        viewInfo.subresourceRange.baseMipLevel =
            0;


        viewInfo.subresourceRange.levelCount =
            1;


        viewInfo.subresourceRange.baseArrayLayer =
            0;


        viewInfo.subresourceRange.layerCount =
            1;



        if (vkCreateImageView(
            m_Device,
            &viewInfo,
            nullptr,
            &m_ImageView)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create depth image view!"
            );
        }

    }



    void VulkanDepthBuffer::Shutdown()
    {

        if (m_ImageView)
        {
            vkDestroyImageView(
                m_Device,
                m_ImageView,
                nullptr
            );
        }


        if (m_Image)
        {
            vkDestroyImage(
                m_Device,
                m_Image,
                nullptr
            );
        }


        if (m_Memory)
        {
            vkFreeMemory(
                m_Device,
                m_Memory,
                nullptr
            );
        }

    }



    uint32_t VulkanDepthBuffer::FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    )
    {

        VkPhysicalDeviceMemoryProperties memProperties{};


        vkGetPhysicalDeviceMemoryProperties(
            m_PhysicalDevice,
            &memProperties
        );



        for (uint32_t i = 0;
            i < memProperties.memoryTypeCount;
            i++)
        {

            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties)
                == properties)
            {
                return i;
            }

        }


        throw std::runtime_error(
            "Failed to find depth memory type!"
        );

    }


}