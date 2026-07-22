#pragma once


#include "VulkanBuffer.h"

#include <vector>


namespace Daybreak
{


    class VulkanIndexBuffer
    {

    public:


        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            const std::vector<uint32_t>& indices
        );


        void Shutdown();



        VkBuffer GetBuffer() const
        {
            return m_Buffer.GetBuffer();
        }



    private:

        VulkanBuffer m_Buffer;


    };


}