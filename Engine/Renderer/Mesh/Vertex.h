#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace Daybreak
{

    // 顶点数据结构
    // 一个 Vertex 对应模型中的一个顶点
    struct Vertex
    {
        // 顶点位置
        glm::vec3 Position;

        // 顶点颜色
        glm::vec3 Color;

        // 纹理坐标（后续加载贴图会使用）
        glm::vec2 TexCoord;

        // 返回 Vulkan 顶点绑定描述
        static VkVertexInputBindingDescription
            GetBindingDescription();

        // 返回 Vulkan 顶点属性描述
        static void GetAttributeDescriptions(
            VkVertexInputAttributeDescription attributes[3]
        );
    };

}