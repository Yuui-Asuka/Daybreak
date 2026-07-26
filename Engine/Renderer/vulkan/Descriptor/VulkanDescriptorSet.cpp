#include "VulkanDescriptorSet.h"

#include <stdexcept>


namespace Daybreak
{

    /**
     * @brief Allocates and initializes a Vulkan descriptor set.
     *
     * A descriptor set stores references to GPU resources that are
     * accessed by shaders.
     *
     * The descriptor set layout defines the binding structure:
     *
     * binding 0:
     *     Uniform Buffer
     *
     * binding 1:
     *     Combined Image Sampler
     *
     * The actual resources are assigned through
     * vkUpdateDescriptorSets().
     *
     * @param device Vulkan logical device.
     * @param descriptorPool Descriptor pool used for allocation.
     * @param layout Descriptor set layout describing bindings.
     * @param uniformBuffer Buffer bound to the uniform buffer binding.
     * @param bufferSize Size of the uniform buffer range.
     * @param textureInfo Image sampler information.
     */
    void VulkanDescriptorSet::Init(
        VkDevice device,
        VkDescriptorPool descriptorPool,
        VkDescriptorSetLayout layout,
        VkBuffer uniformBuffer,
        VkDeviceSize bufferSize,
        const VkDescriptorImageInfo& textureInfo
    )
    {

        m_Device = device;



        /**
         * @brief Allocate descriptor set from descriptor pool.
         *
         * DescriptorSetLayout defines the expected resource bindings,
         * while DescriptorPool provides the memory used for allocation.
         */
        VkDescriptorSetAllocateInfo allocInfo{};


        allocInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;


        allocInfo.descriptorPool =
            descriptorPool;


        allocInfo.descriptorSetCount =
            1;


        allocInfo.pSetLayouts =
            &layout;



        if (vkAllocateDescriptorSets(
            device,
            &allocInfo,
            &m_DescriptorSet)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to allocate descriptor set!"
            );
        }



        /**
         * @brief Describe uniform buffer binding.
         *
         * This connects shader binding 0 with the Vulkan buffer.
         */
        VkDescriptorBufferInfo bufferInfo{};


        bufferInfo.buffer =
            uniformBuffer;


        bufferInfo.offset =
            0;


        bufferInfo.range =
            bufferSize;



        /**
         * @brief Write uniform buffer binding.
         */
        VkWriteDescriptorSet bufferWrite{};


        bufferWrite.sType =
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;


        bufferWrite.dstSet =
            m_DescriptorSet;


        bufferWrite.dstBinding =
            0;


        bufferWrite.dstArrayElement =
            0;


        bufferWrite.descriptorType =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;


        bufferWrite.descriptorCount =
            1;


        bufferWrite.pBufferInfo =
            &bufferInfo;



        /**
         * @brief Write texture sampler binding.
         *
         * This connects shader binding 1 with the image sampler.
         */
        VkWriteDescriptorSet textureWrite{};


        textureWrite.sType =
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;


        textureWrite.dstSet =
            m_DescriptorSet;


        textureWrite.dstBinding =
            1;


        textureWrite.dstArrayElement =
            0;


        textureWrite.descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;


        textureWrite.descriptorCount =
            1;


        textureWrite.pImageInfo =
            &textureInfo;



        VkWriteDescriptorSet writes[] =
        {
            bufferWrite,
            textureWrite
        };



        /**
         * @brief Update descriptor set bindings.
         *
         * After this call, shaders can access the bound resources
         * through the corresponding descriptor bindings.
         */
        vkUpdateDescriptorSets(
            device,
            2,
            writes,
            0,
            nullptr
        );

    }



    /**
     * @brief Releases descriptor set reference.
     *
     * Descriptor sets are owned by the descriptor pool and are
     * automatically released when the pool is destroyed.
     */
    void VulkanDescriptorSet::Shutdown()
    {

        m_DescriptorSet =
            VK_NULL_HANDLE;

    }

}