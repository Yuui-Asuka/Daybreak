#include "VulkanBuffer.h"

#include <stdexcept>
#include <cstring>


namespace Daybreak
{

    /**
     * @brief Creates and initializes a Vulkan buffer.
     *
     * A Vulkan buffer consists of two separate resources:
     *
     * - VkBuffer:
     *   Describes the buffer resource and its usage.
     *
     * - VkDeviceMemory:
     *   Stores the actual memory allocated by the GPU.
     *
     * The allocated memory is bound to the buffer after creation.
     *
     * @param device Vulkan logical device.
     * @param physicalDevice Vulkan physical device used for memory queries.
     * @param size Buffer size in bytes.
     * @param usage Buffer usage flags.
     */
    void VulkanBuffer::Init(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage)
    {
        m_Device = device;

        m_PhysicalDevice =
            physicalDevice;


        /**
         * @brief Buffer creation configuration.
         *
         * Defines the size, usage, and sharing mode of the buffer.
         */
        VkBufferCreateInfo bufferInfo{};

        bufferInfo.sType =
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;


        bufferInfo.size =
            size;


        bufferInfo.usage =
            usage;


        bufferInfo.sharingMode =
            VK_SHARING_MODE_EXCLUSIVE;



        if (vkCreateBuffer(
            m_Device,
            &bufferInfo,
            nullptr,
            &m_Buffer)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create buffer!"
            );
        }


        /**
         * @brief Queries memory requirements for the buffer.
         *
         * The requirements contain:
         *
         * - Required allocation size.
         * - Supported memory type bits.
         */
        VkMemoryRequirements requirements{};


        vkGetBufferMemoryRequirements(
            m_Device,
            m_Buffer,
            &requirements
        );


        /**
         * @brief Memory allocation information.
         */
        VkMemoryAllocateInfo allocateInfo{};


        allocateInfo.sType =
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;


        allocateInfo.allocationSize =
            requirements.size;


        /**
         * @brief Select CPU-visible coherent memory.
         *
         * HOST_VISIBLE allows CPU mapping.
         *
         * HOST_COHERENT avoids explicit cache flushing after writes.
         *
         * This is suitable for simple CPU-uploaded buffers.
         */
        allocateInfo.memoryTypeIndex =
            FindMemoryType(
                requirements.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );


        if (vkAllocateMemory(
            m_Device,
            &allocateInfo,
            nullptr,
            &m_Memory)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to allocate buffer memory!"
            );
        }


        /**
         * @brief Associates allocated memory with the buffer.
         *
         * The buffer cannot be used until memory is bound.
         */
        vkBindBufferMemory(
            m_Device,
            m_Buffer,
            m_Memory,
            0
        );

    }



    /**
     * @brief Uploads data from CPU memory into the buffer.
     *
     * Maps the allocated device memory, copies the data,
     * and unmaps the memory afterwards.
     *
     * @param data Source data pointer.
     * @param size Number of bytes to upload.
     */
    void VulkanBuffer::Upload(
        const void* data,
        VkDeviceSize size)
    {
        void* mapped = nullptr;

        vkMapMemory(
            m_Device,
            m_Memory,
            0,
            size,
            0,
            &mapped
        );

        memcpy(
            mapped,
            data,
            size
        );

        vkUnmapMemory(
            m_Device,
            m_Memory
        );
    }



    /**
     * @brief Finds a compatible Vulkan memory type.
     *
     * Vulkan exposes multiple memory types with different
     * properties. This function searches for one matching:
     *
     * - Supported memory type bits.
     * - Required memory properties.
     *
     * @param typeFilter Bit mask of supported memory types.
     * @param properties Required memory properties.
     *
     * @return Index of a suitable memory type.
     */
    uint32_t VulkanBuffer::FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties)
    {

        VkPhysicalDeviceMemoryProperties memoryProperties{};

        vkGetPhysicalDeviceMemoryProperties(
            m_PhysicalDevice,
            &memoryProperties
        );

        for (uint32_t i = 0;
            i < memoryProperties.memoryTypeCount;
            i++)
        {

            if ((typeFilter & (1 << i)) &&
                (memoryProperties.memoryTypes[i].propertyFlags
                    & properties)
                == properties)
            {
                return i;
            }

        }

        throw std::runtime_error(
            "Failed to find suitable memory type!"
        );
    }


    /**
     * @brief Releases Vulkan buffer resources.
     *
     * The buffer must be destroyed before releasing the
     * associated device memory.
     */
    void VulkanBuffer::Shutdown()
    {

        if (m_Buffer != VK_NULL_HANDLE)
        {

            vkDestroyBuffer(
                m_Device,
                m_Buffer,
                nullptr
            );


            m_Buffer =
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
}