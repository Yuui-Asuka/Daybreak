#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @brief Vulkan descriptor set wrapper.
     *
     * A descriptor set stores references to GPU resources that are
     * accessed by shaders.
     *
     * Descriptor sets are allocated from a descriptor pool and are
     * configured according to a descriptor set layout.
     *
     * Current bindings:
     *
     * Binding 0:
     *     Uniform Buffer
     *
     * Binding 1:
     *     Combined Image Sampler
     *
     * The descriptor set itself does not own the underlying resources.
     * Buffers and images are managed by their respective resource classes.
     */
    class VulkanDescriptorSet
    {

    public:

        /**
         * @brief Allocates and updates a descriptor set.
         *
         * @param device Vulkan logical device.
         * @param descriptorPool Descriptor pool used for allocation.
         * @param layout Descriptor set layout defining bindings.
         * @param uniformBuffer Buffer bound to the uniform binding.
         * @param bufferSize Size of the uniform buffer range.
         * @param textureInfo Image sampler information.
         */
        void Init(
            VkDevice device,
            VkDescriptorPool descriptorPool,
            VkDescriptorSetLayout layout,
            VkBuffer uniformBuffer,
            VkDeviceSize bufferSize,
            const VkDescriptorImageInfo& textureInfo
        );



        /**
         * @brief Releases descriptor set reference.
         *
         * The descriptor set is owned by the descriptor pool and does
         * not require explicit destruction.
         */
        void Shutdown();



        /**
         * @brief Returns the Vulkan descriptor set handle.
         *
         * Used when binding resources during command recording:
         *
         * vkCmdBindDescriptorSets()
         *
         * @return VkDescriptorSet handle.
         */
        VkDescriptorSet GetDescriptorSet() const
        {
            return m_DescriptorSet;
        }



    private:

        /**
         * @brief Cached texture descriptor information.
         *
         * Stores image and sampler information used for descriptor
         * updates.
         */
        VkDescriptorImageInfo m_TextureInfo{};



        /**
         * @brief Vulkan logical device.
         *
         * Used for descriptor related operations.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;



        /**
         * @brief Vulkan descriptor set handle.
         *
         * Allocated from VkDescriptorPool.
         */
        VkDescriptorSet m_DescriptorSet =
            VK_NULL_HANDLE;

    };

}