#include "Mesh.h"


namespace Daybreak
{

    Mesh::Mesh(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices
    )
        :
        m_Vertices(vertices),
            m_Indices(indices)
    {}


    Mesh Mesh::CreateTriangle()
    {
        std::vector<Vertex> vertices =
        {
            // 0 左上
            {
                {-0.8f, 0.8f, 0.0f},
                {1.0f,0.0f,0.0f}
            },

            // 1 右上
            {
                {0.8f,0.8f,0.0f},
                {0.0f,1.0f,0.0f}
            },

            // 2 右下
            {
                {0.8f,-0.8f,0.0f},
                {0.0f,0.0f,1.0f}
            },

            // 3 左下
            {
                {-0.8f,-0.8f,0.0f},
                {1.0f,1.0f,1.0f}
            }
        };


        std::vector<uint32_t> indices =
        {
            0,1,2,   // 第一个三角形

            2,3,0    // 第二个三角形
        };


        return Mesh(
            vertices,
            indices
        );
    }

}