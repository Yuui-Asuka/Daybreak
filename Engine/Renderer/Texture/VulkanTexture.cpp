#include "VulkanTexture.h"

#include <stdexcept>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../../ThirdParty/stb/stb_image.h"


namespace Daybreak
{

    void VulkanTexture::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkCommandPool commandPool,
        VkQueue graphicsQueue,
        const char* path
    )
    {

        m_Device = device;

        m_PhysicalDevice =
            physicalDevice;

        m_CommandPool = commandPool;

        m_GraphicsQueue = graphicsQueue;


        LoadImage(path);



        VkDeviceSize imageSize =
            m_Width *
            m_Height *
            4;



        CreateStagingBuffer(
            m_Pixels,
            imageSize
        );



        CreateImage();



        TransitionImageLayout(
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );



        CopyBufferToImage();



        //TransitionImageLayout(
        //    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        //    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        //);



        CreateImageView();



        CreateSampler();

    }

    void VulkanTexture::CreateImage()
    {

        VkImageCreateInfo imageInfo{};


        imageInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;


        imageInfo.imageType =
            VK_IMAGE_TYPE_2D;


        imageInfo.extent.width =
            static_cast<uint32_t>(m_Width);


        imageInfo.extent.height =
            static_cast<uint32_t>(m_Height);


        imageInfo.extent.depth =
            1;


        imageInfo.mipLevels =
            1;


        imageInfo.arrayLayers =
            1;


        imageInfo.format =
            VK_FORMAT_R8G8B8A8_SRGB;


        imageInfo.tiling =
            VK_IMAGE_TILING_OPTIMAL;


        imageInfo.initialLayout =
            VK_IMAGE_LAYOUT_UNDEFINED;


        imageInfo.usage =
            VK_IMAGE_USAGE_TRANSFER_DST_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT;


        imageInfo.samples =
            VK_SAMPLE_COUNT_1_BIT;


        imageInfo.sharingMode =
            VK_SHARING_MODE_EXCLUSIVE;



        /*
            创建 VkImage 对象

            注意：
            这里只创建 Image 句柄，
            还没有分配显存。
        */

        if (vkCreateImage(
            m_Device,
            &imageInfo,
            nullptr,
            &m_Image)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed create texture image"
            );
        }



        /*
            获取 Image 需要多少显存

        */

        VkMemoryRequirements memRequirements{};


        vkGetImageMemoryRequirements(
            m_Device,
            m_Image,
            &memRequirements
        );



        /*
            分配 GPU 显存

        */

        VkMemoryAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;


        allocInfo.allocationSize =
            memRequirements.size;


        allocInfo.memoryTypeIndex =
            FindMemoryType(
                memRequirements.memoryTypeBits,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );



        if (vkAllocateMemory(
            m_Device,
            &allocInfo,
            nullptr,
            &m_ImageMemory)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed allocate texture image memory"
            );
        }



        /*
            最关键：

            把 VkImage 绑定到 VkDeviceMemory

        */

        if (vkBindImageMemory(
            m_Device,
            m_Image,
            m_ImageMemory,
            0)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed bind image memory"
            );
        }

    }

    void VulkanTexture::CreateImageView()
    {

        VkImageViewCreateInfo viewInfo{};


        viewInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;


        viewInfo.image =
            m_Image;


        viewInfo.viewType =
            VK_IMAGE_VIEW_TYPE_2D;


        viewInfo.format =
            VK_FORMAT_R8G8B8A8_SRGB;


        viewInfo.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;


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
                "Failed create texture view"
            );
        }

    }

    uint32_t Daybreak::VulkanTexture::FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    )
    {

        VkPhysicalDeviceMemoryProperties memProperties;


        vkGetPhysicalDeviceMemoryProperties(
            m_PhysicalDevice,
            &memProperties
        );


        for (uint32_t i = 0;
            i < memProperties.memoryTypeCount;
            i++)
        {

            if (
                (typeFilter & (1 << i))
                &&
                (memProperties.memoryTypes[i].propertyFlags & properties)
                ==
                properties
                )
            {
                return i;
            }

        }


        throw std::runtime_error(
            "Failed to find suitable memory type!"
        );

    }

    void VulkanTexture::CreateStagingBuffer(
        unsigned char* pixels,
        VkDeviceSize size
    )
    {

        VkBufferCreateInfo bufferInfo{};


        bufferInfo.sType =
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;


        bufferInfo.size =
            size;


        bufferInfo.usage =
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT;


        bufferInfo.sharingMode =
            VK_SHARING_MODE_EXCLUSIVE;



        vkCreateBuffer(
            m_Device,
            &bufferInfo,
            nullptr,
            &m_StagingBuffer
        );



        VkMemoryRequirements memRequirements;


        vkGetBufferMemoryRequirements(
            m_Device,
            m_StagingBuffer,
            &memRequirements
        );



        VkMemoryAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;


        allocInfo.allocationSize =
            memRequirements.size;


        allocInfo.memoryTypeIndex =
            FindMemoryType(
                memRequirements.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );



        vkAllocateMemory(
            m_Device,
            &allocInfo,
            nullptr,
            &m_StagingMemory
        );


        vkBindBufferMemory(
            m_Device,
            m_StagingBuffer,
            m_StagingMemory,
            0
        );



        void* data;


        vkMapMemory(
            m_Device,
            m_StagingMemory,
            0,
            size,
            0,
            &data
        );


        memcpy(
            data,
            pixels,
            size
        );


        vkUnmapMemory(
            m_Device,
            m_StagingMemory
        );

    }



    void VulkanTexture::CreateSampler()
    {

        VkSamplerCreateInfo samplerInfo{};


        samplerInfo.sType =
            VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;


        samplerInfo.magFilter =
            VK_FILTER_LINEAR;


        samplerInfo.minFilter =
            VK_FILTER_LINEAR;


        samplerInfo.addressModeU =
            VK_SAMPLER_ADDRESS_MODE_REPEAT;


        samplerInfo.addressModeV =
            VK_SAMPLER_ADDRESS_MODE_REPEAT;


        samplerInfo.addressModeW =
            VK_SAMPLER_ADDRESS_MODE_REPEAT;


        samplerInfo.anisotropyEnable =
            VK_FALSE;


        samplerInfo.borderColor =
            VK_BORDER_COLOR_INT_OPAQUE_BLACK;


        samplerInfo.unnormalizedCoordinates =
            VK_FALSE;


        samplerInfo.compareEnable =
            VK_FALSE;


        samplerInfo.mipmapMode =
            VK_SAMPLER_MIPMAP_MODE_LINEAR;



        if (vkCreateSampler(
            m_Device,
            &samplerInfo,
            nullptr,
            &m_Sampler)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed create sampler"
            );
        }

    }

    void VulkanTexture::LoadImage(
        const char* path
    )
    {

        m_Pixels =
            stbi_load(
                path,
                &m_Width,
                &m_Height,
                &m_Channels,
                STBI_rgb_alpha
            );


        if (!m_Pixels)
        {
            throw std::runtime_error(
                "Failed to load texture"
            );
        }


        std::cout
            << "Texture loaded: "
            << m_Width
            << " x "
            << m_Height
            << std::endl;

    }


    void VulkanTexture::TransitionImageLayout(
        VkImageLayout oldLayout,
        VkImageLayout newLayout
    )
    {

        std::cout
            << "Transition Image: "
            << m_Image
            << std::endl;

        std::cout
            << "oldLayout="
            << oldLayout
            << " newLayout="
            << newLayout
            << std::endl;

        VkCommandBuffer commandBuffer =
            BeginSingleTimeCommands();



        VkImageMemoryBarrier barrier{};


        barrier.sType =
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;


        barrier.oldLayout =
            oldLayout;


        barrier.newLayout =
            newLayout;


        barrier.srcQueueFamilyIndex =
            VK_QUEUE_FAMILY_IGNORED;


        barrier.dstQueueFamilyIndex =
            VK_QUEUE_FAMILY_IGNORED;


        barrier.image =
            m_Image;


        barrier.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;


        barrier.subresourceRange.baseMipLevel =
            0;


        barrier.subresourceRange.levelCount =
            1;


        barrier.subresourceRange.baseArrayLayer =
            0;


        barrier.subresourceRange.layerCount =
            1;


        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;



        if (
            oldLayout ==
            VK_IMAGE_LAYOUT_UNDEFINED
            &&
            newLayout ==
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
            )
        {

            barrier.srcAccessMask =
                0;


            barrier.dstAccessMask =
                VK_ACCESS_TRANSFER_WRITE_BIT;


            sourceStage =
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;


            destinationStage =
                VK_PIPELINE_STAGE_TRANSFER_BIT;

        }


        else if (
            oldLayout ==
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
            &&
            newLayout ==
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            )
        {

            barrier.srcAccessMask =
                VK_ACCESS_TRANSFER_WRITE_BIT;


            barrier.dstAccessMask =
                VK_ACCESS_SHADER_READ_BIT;


            sourceStage =
                VK_PIPELINE_STAGE_TRANSFER_BIT;


            destinationStage =
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        }


        else
        {
            throw std::runtime_error(
                "Unsupported layout transition"
            );
        }



        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage,
            destinationStage,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier
        );



        EndSingleTimeCommands(
            commandBuffer
        );

    }

    void VulkanTexture::CopyBufferToImage()
    {

        VkCommandBuffer commandBuffer =
            BeginSingleTimeCommands();



        VkBufferImageCopy region{};


        region.bufferOffset = 0;


        region.bufferRowLength = 0;


        region.bufferImageHeight = 0;



        region.imageSubresource.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;


        region.imageSubresource.mipLevel =
            0;


        region.imageSubresource.baseArrayLayer =
            0;


        region.imageSubresource.layerCount =
            1;



        region.imageOffset =
        {
            0,
            0,
            0
        };


        region.imageExtent =
        {
            static_cast<uint32_t>(m_Width),
            static_cast<uint32_t>(m_Height),
            1
        };



        vkCmdCopyBufferToImage(
            commandBuffer,
            m_StagingBuffer,
            m_Image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );



        EndSingleTimeCommands(
            commandBuffer
        );

    }

    void VulkanTexture::DestroyStagingBuffer()
    {

        if (m_StagingBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(
                m_Device,
                m_StagingBuffer,
                nullptr
            );

            m_StagingBuffer =
                VK_NULL_HANDLE;
        }


        if (m_StagingMemory != VK_NULL_HANDLE)
        {
            vkFreeMemory(
                m_Device,
                m_StagingMemory,
                nullptr
            );

            m_StagingMemory =
                VK_NULL_HANDLE;
        }

        stbi_image_free(m_Pixels);


        m_Pixels = nullptr;

    }


    void VulkanTexture::Shutdown()
    {

        if (m_Sampler)
        {
            vkDestroySampler(
                m_Device,
                m_Sampler,
                nullptr);
        }


        if (m_ImageView)
        {
            vkDestroyImageView(
                m_Device,
                m_ImageView,
                nullptr);
        }


        if (m_Image)
        {
            vkDestroyImage(
                m_Device,
                m_Image,
                nullptr);
        }


        if (m_ImageMemory)
        {
            vkFreeMemory(
                m_Device,
                m_ImageMemory,
                nullptr);
        }

    }

    VkDescriptorImageInfo Daybreak::VulkanTexture::GetDescriptorInfo() const
    {

        std::cout
            << "Inside GetDescriptorInfo\n";

        std::cout
            << "m_ImageView = "
            << m_ImageView
            << std::endl;


        std::cout
            << "m_Sampler = "
            << m_Sampler
            << std::endl;


        VkDescriptorImageInfo info{};

        info.imageLayout =
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        info.imageView =
            m_ImageView;

        info.sampler =
            m_Sampler;


        std::cout
            << "info.imageView = "
            << info.imageView
            << std::endl;


        std::cout
            << "info.sampler = "
            << info.sampler
            << std::endl;


        return info;
    }

    VkCommandBuffer VulkanTexture::BeginSingleTimeCommands()
    {

        std::cout
            << "Device="
            << m_Device
            << std::endl;


        std::cout
            << "CommandPool="
            << m_CommandPool
            << std::endl;

        VkCommandBufferAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;


        allocInfo.level =
            VK_COMMAND_BUFFER_LEVEL_PRIMARY;


        allocInfo.commandPool =
            m_CommandPool;


        allocInfo.commandBufferCount =
            1;


        VkCommandBuffer commandBuffer;


        VkResult result = vkAllocateCommandBuffers(
            m_Device,
            &allocInfo,
            &commandBuffer
        );

        std::cout
            << "Allocate result="
            << result
            << std::endl;

        VkCommandBufferBeginInfo beginInfo{};


        beginInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


        beginInfo.flags =
            VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;



        vkBeginCommandBuffer(
            commandBuffer,
            &beginInfo
        );


        return commandBuffer;
    }

    void VulkanTexture::EndSingleTimeCommands(
        VkCommandBuffer commandBuffer
    )
    {

        vkEndCommandBuffer(
            commandBuffer
        );


        VkSubmitInfo submitInfo{};


        submitInfo.sType =
            VK_STRUCTURE_TYPE_SUBMIT_INFO;


        submitInfo.commandBufferCount =
            1;


        submitInfo.pCommandBuffers =
            &commandBuffer;



        //vkQueueSubmit(
        //    m_GraphicsQueue,
        //    1,
        //    &submitInfo,
        //    VK_NULL_HANDLE
        //);

        VkResult result =
            vkQueueSubmit(
                m_GraphicsQueue,
                1,
                &submitInfo,
                VK_NULL_HANDLE
            );


        std::cout
            << "Texture submit result = "
            << result
            << std::endl;


        vkQueueWaitIdle(
            m_GraphicsQueue
        );


        vkFreeCommandBuffers(
            m_Device,
            m_CommandPool,
            1,
            &commandBuffer
        );

    }

}