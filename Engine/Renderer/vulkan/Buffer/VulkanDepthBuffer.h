#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @brief Manages a Vulkan depth buffer resource.
     *
     * A depth buffer is used during rasterization to perform depth
     * testing and determine fragment visibility.
     *
     * The resource consists of:
     *
     * - VkImage:
     *   Stores depth values generated during rendering.
     *
     * - VkDeviceMemory:
     *   GPU memory backing the depth image.
     *
     * - VkImageView:
     *   Provides access to the image as a render pass attachment.
     */
    class VulkanDepthBuffer
    {

    public:

        /**
         * @brief Creates the depth buffer resources.
         *
         * Allocates a GPU-local depth image, binds memory to it,
         * and creates the corresponding image view.
         *
         * @param device Vulkan logical device.
         * @param physicalDevice Physical device used for memory queries.
         * @param extent Depth buffer resolution.
         */
        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkExtent2D extent
        );


        /**
         * @brief Releases depth buffer resources.
         *
         * Destroys:
         *
         * - Image view.
         * - Image.
         * - Allocated device memory.
         */
        void Shutdown();


        /**
         * @brief Returns the depth image view.
         *
         * The image view is used as the depth attachment when creating
         * framebuffers.
         *
         * @return Vulkan image view handle.
         */
        VkImageView GetImageView() const
        {
            return m_ImageView;
        }


    private:

        /**
         * @brief Vulkan logical device used for resource management.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /**
         * @brief Physical device used for querying memory properties.
         */
        VkPhysicalDevice m_PhysicalDevice =
            VK_NULL_HANDLE;


        /**
         * @brief Depth image resource.
         */
        VkImage m_Image =
            VK_NULL_HANDLE;


        /**
         * @brief Memory allocation backing the depth image.
         */
        VkDeviceMemory m_Memory =
            VK_NULL_HANDLE;


        /**
         * @brief View of the depth image used by rendering operations.
         */
        VkImageView m_ImageView =
            VK_NULL_HANDLE;



    private:

        /**
         * @brief Finds a compatible Vulkan memory type.
         *
         * Searches the physical device memory properties for a type
         * matching the requested memory requirements.
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


    };

}