#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>

namespace Daybreak
{

    /**
     * @brief Vulkan Buffer 封装类
     *
     * 通用 GPU Buffer
     *
     * 支持:
     *
     * Vertex Buffer
     * Index Buffer
     * Uniform Buffer
     *
     */
    class VulkanBuffer
    {
    public:

        /**
         * @brief 创建 Vulkan Buffer
         *
         * @param size Buffer 大小
         * @param usage Buffer用途
         */
        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkDeviceSize size,
            VkBufferUsageFlags usage
        );


        /**
         * @brief 上传 CPU 数据到 GPU
         *
         */
        void Upload(
            const void* data,
            VkDeviceSize size
        );


        /**
         * @brief 销毁 Buffer
         */
        void Shutdown();



        /**
         * @brief 获取 VkBuffer
         */
        VkBuffer GetBuffer() const
        {
            return m_Buffer;
        }


        /**
         * @brief 获取 Memory
         */
        VkDeviceMemory GetMemory() const
        {
            return m_Memory;
        }



    private:


        /**
         * @brief 查找 Memory 类型
         */
        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        );



    private:

        VkDevice m_Device =
            VK_NULL_HANDLE;


        VkPhysicalDevice m_PhysicalDevice =
            VK_NULL_HANDLE;


        VkBuffer m_Buffer =
            VK_NULL_HANDLE;


        VkDeviceMemory m_Memory =
            VK_NULL_HANDLE;


    };


}