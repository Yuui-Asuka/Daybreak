#include "VulkanDescriptorSetLayout.h"

#include <stdexcept>


namespace Daybreak
{


    void VulkanDescriptorSetLayout::Init(
        VkDevice device
    )
    {

        m_Device = device;



        /*
            描述一个 Binding。

            对应 Shader:

            layout(binding = 0) uniform UBO

        */

        VkDescriptorSetLayoutBinding uboLayoutBinding{};



        uboLayoutBinding.binding = 0;



        uboLayoutBinding.descriptorType =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;



        uboLayoutBinding.descriptorCount = 1;



        uboLayoutBinding.stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT;



        uboLayoutBinding.pImmutableSamplers =
            nullptr;



        VkDescriptorSetLayoutCreateInfo layoutInfo{};


        layoutInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;



        layoutInfo.bindingCount = 1;


        layoutInfo.pBindings =
            &uboLayoutBinding;



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