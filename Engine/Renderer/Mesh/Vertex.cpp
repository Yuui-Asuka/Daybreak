#include "Vertex.h"

namespace Daybreak
{

    VkVertexInputBindingDescription
        Vertex::GetBindingDescription()
    {
        VkVertexInputBindingDescription binding{};

        // 一个 Vertex 为一组数据
        binding.binding = 0;

        // 每个顶点占多少字节
        binding.stride = sizeof(Vertex);

        // 每读取一个 Vertex 前进一步
        binding.inputRate =
            VK_VERTEX_INPUT_RATE_VERTEX;

        return binding;
    }

    void Vertex::GetAttributeDescriptions(
        VkVertexInputAttributeDescription attributes[3])
    {
        // Position
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format =
            VK_FORMAT_R32G32B32_SFLOAT;
        attributes[0].offset =
            offsetof(Vertex, Position);

        // Color
        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].format =
            VK_FORMAT_R32G32B32_SFLOAT;
        attributes[1].offset =
            offsetof(Vertex, Color);

        // TexCoord
        attributes[2].binding = 0;
        attributes[2].location = 2;
        attributes[2].format =
            VK_FORMAT_R32G32_SFLOAT;
        attributes[2].offset =
            offsetof(Vertex, TexCoord);
    }

}