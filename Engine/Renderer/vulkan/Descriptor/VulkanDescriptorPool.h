#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @brief Vulkan descriptor pool wrapper.
     *
     * A descriptor pool manages the allocation of descriptor sets.
     *
     * Descriptor sets are allocated from a descriptor pool and contain
     * bindings for GPU resources such as:
     *
     * - Uniform buffers.
     * - Storage buffers.
     * - Sampled images.
     * - Samplers.
     *
     * The pool defines the available descriptor types and the maximum
     * number of descriptor sets that can be allocated.
     */
    class VulkanDescriptorPool
    {

    public:

        /**
         * @brief Creates a descriptor pool.
         *
         * @param device Vulkan logical device used for resource creation.
         */
        void Init(
            VkDevice device
        );



        /**
         * @brief Destroys the descriptor pool.
         *
         * Descriptor sets allocated from this pool are automatically
         * released when the pool is destroyed.
         */
        void Shutdown();



        /**
         * @brief Returns the Vulkan descriptor pool handle.
         *
         * Used when allocating descriptor sets with:
         *
         * vkAllocateDescriptorSets()
         *
         * @return VkDescriptorPool handle.
         */
        VkDescriptorPool GetPool() const
        {
            return m_Pool;
        }



    private:

        /**
         * @brief Vulkan logical device.
         *
         * Required for creating and destroying the descriptor pool.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;



        /**
         * @brief Vulkan descriptor pool handle.
         *
         * Used as the source for descriptor set allocation.
         */
        VkDescriptorPool m_Pool =
            VK_NULL_HANDLE;

    };

}