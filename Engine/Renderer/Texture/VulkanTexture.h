#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{


    class VulkanTexture
    {

    public:

        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkCommandPool commandPool,
            VkQueue graphicsQueue,
            const char* path
        );


        void Shutdown();



        VkImageView GetImageView() const
        {
            return m_ImageView;
        }


        VkSampler GetSampler() const
        {
            return m_Sampler;
        }

        VkDescriptorImageInfo GetDescriptorInfo() const;


    private:

        

        VkDevice m_Device =
            VK_NULL_HANDLE;


        VkPhysicalDevice m_PhysicalDevice =
            VK_NULL_HANDLE;


        VkImage m_Image =
            VK_NULL_HANDLE;


        VkDeviceMemory m_ImageMemory =
            VK_NULL_HANDLE;


        VkImageView m_ImageView =
            VK_NULL_HANDLE;


        VkSampler m_Sampler =
            VK_NULL_HANDLE;


        VkBuffer m_StagingBuffer = VK_NULL_HANDLE;

        VkDeviceMemory m_StagingMemory = VK_NULL_HANDLE;


        VkCommandPool m_CommandPool = VK_NULL_HANDLE;;

        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;

        int m_Width = 0;

        int m_Height = 0;

        int m_Channels = 0;


        unsigned char* m_Pixels = nullptr;

    private:

        void CopyBufferToImage();

        void LoadImage(
            const char* path
        );

        void CreateImage();

        void CreateStagingBuffer();

        VkCommandBuffer BeginSingleTimeCommands();

        void EndSingleTimeCommands(
            VkCommandBuffer commandBuffer
        );

        void TransitionImageLayout(
            VkImageLayout oldLayout,
            VkImageLayout newLayout
        );

        void CreateImageView();

        void CreateSampler();

        void CreateStagingBuffer(
            unsigned char* pixels,
            VkDeviceSize size
        );

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        );

        void DestroyStagingBuffer();
    };


}