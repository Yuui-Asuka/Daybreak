#include "Mesh.h"


namespace Daybreak
{

    Mesh::Mesh(
        const std::vector<Vertex>& vertices
    )
    {
        m_Vertices = vertices;
    }


    Mesh Mesh::CreateTriangle()
    {
        std::vector<Vertex> vertices =
        {
            {
            {-0.8f, -0.8f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        },

        {
            {0.8f, -0.8f, 0.0f},
            {0.0f, 1.0f, 0.0f}
        },

        {
            {0.0f, 0.8f, 0.0f},
            {0.0f, 0.0f, 1.0f}
        }
        };


        return Mesh(vertices);
    }

}