#pragma once

#include <vulkan/vulkan.h>
#include "../Mesh/Vertex.h"
#include <vector>
namespace Daybreak
{

    /**
     * @brief Vulkan Buffer 封装类
     *
     * 用于管理 GPU Buffer。
     *
     * Buffer 用途包括：
     *
     * Vertex Buffer:
     *     保存顶点数据
     *
     * Index Buffer:
     *     保存索引数据
     *
     * Uniform Buffer:
     *     保存 shader 参数
     *
     */
    class VulkanBuffer
    {
    public:

        /**
         * @brief 创建 Vulkan Buffer
         *
         */
        void Init(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            const std::vector<Vertex>& vertices
        );


        /**
         * @brief 销毁 Buffer
         *
         * 释放：
         *
         * VkBuffer
         *
         * VkDeviceMemory
         */
        void Shutdown();


        /**
         * @brief 获取 Vulkan Buffer
         *
         * 用于：
         *
         * vkCmdBindVertexBuffers()
         *
         * vkCmdBindIndexBuffer()
         */
        VkBuffer GetBuffer() const
        {
            return m_Buffer;
        }


        /**
         * @brief 获取 Buffer Memory
         */
        VkDeviceMemory GetMemory() const
        {
            return m_Memory;
        }


    private:

        /**
         * @brief 查找合适的 GPU Memory 类型
         *
         * Vulkan GPU 有不同类型显存：
         *
         * Device Local:
         *     GPU高速显存
         *
         * Host Visible:
         *     CPU可以访问
         *
         */
        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        );


    private:

        /**
         * @brief Vulkan Logical Device
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /**
         * @brief Vulkan Buffer
         */
        VkBuffer m_Buffer =
            VK_NULL_HANDLE;


        /**
         * @brief Buffer 对应显存
         */
        VkDeviceMemory m_Memory =
            VK_NULL_HANDLE;


        /**
         * @brief Physical Device
         *
         * 用于查询 GPU Memory 类型
         */
        VkPhysicalDevice m_PhysicalDevice =
            VK_NULL_HANDLE;

    };

}