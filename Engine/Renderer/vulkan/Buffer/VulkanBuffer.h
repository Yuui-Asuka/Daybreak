#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>


namespace Daybreak
{

    /**
     * @brief Wrapper around a Vulkan buffer resource.
     *
     * Vulkan buffers describe GPU-accessible memory regions.
     * The actual memory allocation is managed separately through
     * VkDeviceMemory and must be bound to the buffer.
     *
     * This class provides a common abstraction for:
     *
     * - Vertex buffers.
     * - Index buffers.
     * - Uniform buffers.
     *
     * The current implementation uses CPU-visible memory, which is
     * suitable for simple data uploads and dynamic resources.
     */
    class VulkanBuffer
    {
    public:

        /**
         * @brief Creates and allocates a Vulkan buffer.
         *
         * Creates the VkBuffer object, allocates compatible device
         * memory, and binds the memory to the buffer.
         *
         * @param device Vulkan logical device.
         * @param physicalDevice Physical device used for memory selection.
         * @param size Buffer size in bytes.
         * @param usage Intended buffer usage flags.
         */
        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkDeviceSize size,
            VkBufferUsageFlags usage
        );


        /**
         * @brief Uploads CPU data into the buffer memory.
         *
         * The current implementation maps host-visible memory,
         * copies the provided data, and unmaps the memory.
         *
         * @param data Source data pointer.
         * @param size Number of bytes to upload.
         */
        void Upload(
            const void* data,
            VkDeviceSize size
        );


        /**
         * @brief Releases buffer resources.
         *
         * Destroys the Vulkan buffer and frees the associated memory.
         */
        void Shutdown();



        /**
         * @brief Returns the Vulkan buffer handle.
         *
         * @return VkBuffer handle.
         */
        VkBuffer GetBuffer() const
        {
            return m_Buffer;
        }


        /**
         * @brief Returns the allocated device memory handle.
         *
         * This memory is bound to the Vulkan buffer and stores
         * the actual buffer contents.
         *
         * @return VkDeviceMemory handle.
         */
        VkDeviceMemory GetMemory() const
        {
            return m_Memory;
        }



    private:

        /**
         * @brief Finds a compatible Vulkan memory type.
         *
         * Searches the physical device memory properties for a
         * memory type matching the requested flags.
         *
         * @param typeFilter Supported memory type bit mask.
         * @param properties Required memory properties.
         *
         * @return Compatible memory type index.
         */
        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        );



    private:

        /**
         * @brief Vulkan logical device used for buffer operations.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /**
         * @brief Physical device used for querying memory properties.
         */
        VkPhysicalDevice m_PhysicalDevice =
            VK_NULL_HANDLE;


        /**
         * @brief Vulkan buffer handle.
         */
        VkBuffer m_Buffer =
            VK_NULL_HANDLE;


        /**
         * @brief Memory allocation bound to the buffer.
         */
        VkDeviceMemory m_Memory =
            VK_NULL_HANDLE;


    };


}