#include "VulkanDescriptorPool.h"

#include <stdexcept>


namespace Daybreak
{

    /**
     * @brief Creates a Vulkan descriptor pool.
     *
     * Descriptor pools are used to allocate descriptor sets.
     *
     * The pool specifies:
     *
     * - Which descriptor types can be allocated.
     * - How many descriptors of each type are available.
     * - Maximum number of descriptor sets.
     *
     * Current configuration supports:
     *
     * - Uniform buffers.
     * - Combined image samplers.
     *
     * This is enough for basic rendering with:
     *
     * - Camera matrices.
     * - Texture sampling.
     */
    void VulkanDescriptorPool::Init(
        VkDevice device
    )
    {

        m_Device = device;



        /**
         * @brief Descriptor capacity configuration.
         *
         * Defines the number of descriptors that can be allocated
         * from this pool.
         *
         * These values do not create descriptor sets directly.
         */
        VkDescriptorPoolSize poolSizes[2];


        /*
         * Uniform buffer descriptors.
         *
         * Used for shader data such as:
         *
         * - Model matrix.
         * - View matrix.
         * - Projection matrix.
         */
        poolSizes[0].type =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;


        poolSizes[0].descriptorCount =
            1;



        /*
         * Combined image sampler descriptors.
         *
         * Used for sampled textures.
         */
        poolSizes[1].type =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;


        poolSizes[1].descriptorCount =
            1;



        VkDescriptorPoolCreateInfo poolInfo{};


        poolInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;



        /*
         * Number of descriptor pool size entries.
         */
        poolInfo.poolSizeCount =
            2;


        poolInfo.pPoolSizes =
            poolSizes;



        /**
         * @brief Maximum number of descriptor sets.
         *
         * The application can allocate at most this many descriptor
         * sets from this pool.
         */
        poolInfo.maxSets =
            1;



        if (vkCreateDescriptorPool(
            m_Device,
            &poolInfo,
            nullptr,
            &m_Pool)
            != VK_SUCCESS)
        {

            throw std::runtime_error(
                "Failed to create descriptor pool!"
            );

        }

    }



    /**
     * @brief Destroys descriptor pool.
     *
     * Descriptor sets allocated from this pool are automatically
     * released when the pool is destroyed.
     */
    void VulkanDescriptorPool::Shutdown()
    {

        if (m_Pool != VK_NULL_HANDLE)
        {

            vkDestroyDescriptorPool(
                m_Device,
                m_Pool,
                nullptr
            );


            m_Pool =
                VK_NULL_HANDLE;

        }

    }


}