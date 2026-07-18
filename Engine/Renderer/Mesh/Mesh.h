#pragma once

#include "Vertex.h"

#include <vector>

namespace Daybreak
{

    class Mesh
    {
    public:

        Mesh() = default;

        Mesh(
            const std::vector<Vertex>& vertices
        );


        static Mesh CreateTriangle();


        const std::vector<Vertex>& GetVertices() const
        {
            return m_Vertices;
        }


        uint32_t GetVertexCount() const
        {
            return static_cast<uint32_t>(
                m_Vertices.size()
                );
        }


    private:

        std::vector<Vertex> m_Vertices;

    };

}