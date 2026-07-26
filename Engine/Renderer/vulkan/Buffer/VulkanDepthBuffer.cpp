#include "VulkanDepthBuffer.h"

#include <stdexcept>


namespace Daybreak
{

    /**
     * @brief Creates a depth buffer resource.
     *
     * A Vulkan depth buffer consists of:
     *
     * - VkImage:
     *   Stores depth information used during depth testing.
     *
     * - VkDeviceMemory:
     *   Allocated GPU memory backing the image.
     *
     * - VkImageView:
     *   Describes how the image is accessed by the render pass.
     *
     * The depth image is stored in device-local memory because it is
     * exclusively accessed by the GPU during rendering.
     *
     * @param device Vulkan logical device.
     * @param physicalDevice Physical device used for memory selection.
     * @param extent Depth buffer resolution.
     */
    void VulkanDepthBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkExtent2D extent
    )
    {

        m_Device = device;

        m_PhysicalDevice = physicalDevice;



        /**
         * @brief Select depth image format.
         *
         * D32_SFLOAT stores depth values using 32-bit floating point
         * precision.
         */
        VkFormat depthFormat =
            VK_FORMAT_D32_SFLOAT;



        /**
         * @brief Image creation information.
         *
         * Creates a 2D image used as a depth attachment.
         */
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


        /**
         * @brief Initial image layout.
         *
         * The render pass will transition the image to the required
         * depth attachment layout before rendering.
         */
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



        /**
         * @brief Query required memory properties.
         */
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



        /**
         * @brief Allocate GPU-local memory.
         *
         * Depth images are only accessed by the GPU, therefore
         * device-local memory provides the best performance.
         */
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



        /**
         * @brief Bind allocated memory to the depth image.
         */
        vkBindImageMemory(
            m_Device,
            m_Image,
            m_Memory,
            0
        );



        /**
         * @brief Creates an image view for the depth image.
         *
         * The image view allows the render pass to access the depth
         * attachment.
         */
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



    /**
     * @brief Releases depth buffer resources.
     *
     * Resources must be destroyed in reverse dependency order:
     *
     * ImageView
     *     |
     * Image
     *     |
     * Device Memory
     */
    void VulkanDepthBuffer::Shutdown()
    {

        if (m_ImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(
                m_Device,
                m_ImageView,
                nullptr
            );

            m_ImageView =
                VK_NULL_HANDLE;
        }


        if (m_Image != VK_NULL_HANDLE)
        {
            vkDestroyImage(
                m_Device,
                m_Image,
                nullptr
            );

            m_Image =
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



    /**
     * @brief Finds a compatible Vulkan memory type.
     *
     * Searches the physical device memory properties for a memory
     * type matching the requested requirements.
     *
     * @param typeFilter Supported memory type bit mask.
     * @param properties Required memory properties.
     *
     * @return Compatible memory type index.
     */
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