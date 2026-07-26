#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{

    /**
     * @brief Vulkan descriptor set layout wrapper.
     *
     * A descriptor set layout defines the structure of resources
     * that can be bound to shaders.
     *
     * It describes:
     *
     * - Descriptor binding index.
     * - Descriptor type.
     * - Number of descriptors.
     * - Shader stages that access the resource.
     *
     * Example:
     *
     * Binding 0:
     *     Uniform Buffer
     *
     * Binding 1:
     *     Combined Image Sampler
     *
     * The descriptor set layout does not contain actual resources.
     * Resources are provided later through VkDescriptorSet.
     */
    class VulkanDescriptorSetLayout
    {

    public:

        /**
         * @brief Creates the descriptor set layout.
         *
         * @param device Vulkan logical device.
         */
        void Init(
            VkDevice device
        );



        /**
         * @brief Destroys the descriptor set layout.
         *
         * The layout should be destroyed only after all objects
         * depending on it, such as pipeline layouts and pipelines,
         * have been released.
         */
        void Shutdown();



        /**
         * @brief Returns the Vulkan descriptor set layout handle.
         *
         * Used when creating:
         *
         * VkPipelineLayout
         *
         * and allocating:
         *
         * VkDescriptorSet
         *
         * @return VkDescriptorSetLayout handle.
         */
        VkDescriptorSetLayout GetLayout() const
        {
            return m_Layout;
        }



    private:

        /**
         * @brief Vulkan logical device.
         *
         * Required for creating and destroying the descriptor
         * set layout.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;



        /**
         * @brief Vulkan descriptor set layout handle.
         *
         * Defines the resource binding layout used by shaders.
         */
        VkDescriptorSetLayout m_Layout =
            VK_NULL_HANDLE;

    };

}