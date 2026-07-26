#include "VulkanDescriptorSetLayout.h"

#include <stdexcept>


namespace Daybreak
{

    /**
     * @brief Creates a Vulkan descriptor set layout.
     *
     * A descriptor set layout defines the structure of resources
     * that can be bound to a shader.
     *
     * It describes:
     *
     * - Descriptor binding index.
     * - Descriptor type.
     * - Descriptor count.
     * - Shader stages that access the resource.
     *
     * Current layout:
     *
     * Binding 0:
     *     Uniform Buffer
     *     Vertex Shader access
     *
     * Binding 1:
     *     Combined Image Sampler
     *     Fragment Shader access
     *
     * The layout must match the descriptor declarations in GLSL.
     */
    void VulkanDescriptorSetLayout::Init(
        VkDevice device
    )
    {

        m_Device = device;



        /**
         * @brief Uniform buffer binding.
         *
         * Matches shader declaration:
         *
         * layout(binding = 0) uniform UniformBufferObject
         */
        VkDescriptorSetLayoutBinding uboLayoutBinding{};


        uboLayoutBinding.binding =
            0;


        uboLayoutBinding.descriptorType =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;


        uboLayoutBinding.descriptorCount =
            1;


        /*
         * UBO is accessed from the vertex shader.
         */
        uboLayoutBinding.stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT;


        uboLayoutBinding.pImmutableSamplers =
            nullptr;



        /**
         * @brief Texture sampler binding.
         *
         * Matches shader declaration:
         *
         * layout(binding = 1) uniform sampler2D textureSampler;
         */
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};


        samplerLayoutBinding.binding =
            1;


        samplerLayoutBinding.descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;


        samplerLayoutBinding.descriptorCount =
            1;


        /*
         * Texture sampling is performed in the fragment shader.
         */
        samplerLayoutBinding.stageFlags =
            VK_SHADER_STAGE_FRAGMENT_BIT;


        samplerLayoutBinding.pImmutableSamplers =
            nullptr;



        VkDescriptorSetLayoutBinding bindings[] =
        {
            uboLayoutBinding,
            samplerLayoutBinding
        };



        /**
         * @brief Descriptor set layout creation information.
         */
        VkDescriptorSetLayoutCreateInfo layoutInfo{};


        layoutInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;


        layoutInfo.bindingCount =
            2;


        layoutInfo.pBindings =
            bindings;



        if (vkCreateDescriptorSetLayout(
            m_Device,
            &layoutInfo,
            nullptr,
            &m_Layout)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create descriptor set layout!"
            );
        }

    }



    /**
     * @brief Destroys descriptor set layout.
     *
     * The layout must be destroyed after all descriptor sets
     * and pipelines using it are no longer needed.
     */
    void VulkanDescriptorSetLayout::Shutdown()
    {

        if (m_Layout != VK_NULL_HANDLE)
        {

            vkDestroyDescriptorSetLayout(
                m_Device,
                m_Layout,
                nullptr
            );


            m_Layout =
                VK_NULL_HANDLE;

        }

    }

}