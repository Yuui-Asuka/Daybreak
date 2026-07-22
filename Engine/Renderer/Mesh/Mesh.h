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
            const std::vector<Vertex>& vertices,
            const std::vector<uint32_t>& indices
        );


        static Mesh CreateTriangle();


        const std::vector<Vertex>& GetVertices() const
        {
            return m_Vertices;
        }

        const std::vector<uint32_t>& GetIndices() const
        {
            return m_Indices;
        }

        uint32_t GetVertexCount() const
        {
            return static_cast<uint32_t>(
                m_Vertices.size()
                );
        }

        uint32_t GetIndexCount() const
        {
            return static_cast<uint32_t>(m_Indices.size());
        }

    private:

        std::vector<Vertex> m_Vertices;

        std::vector<uint32_t> m_Indices;
    };

}