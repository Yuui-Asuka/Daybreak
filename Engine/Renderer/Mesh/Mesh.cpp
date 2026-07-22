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

    Mesh Mesh::CreateCube()
    {


        std::vector<Vertex> vertices =
        {

            {{-0.5f,-0.5f,-0.5f},{1,0,0}},
            {{ 0.5f,-0.5f,-0.5f},{0,1,0}},
            {{ 0.5f, 0.5f,-0.5f},{0,0,1}},
            {{-0.5f, 0.5f,-0.5f},{1,1,0}},


            {{-0.5f,-0.5f,0.5f},{1,0,1}},
            {{ 0.5f,-0.5f,0.5f},{0,1,1}},
            {{ 0.5f,0.5f,0.5f},{1,1,1}},
            {{-0.5f,0.5f,0.5f},{0,0,0}}

        };



        std::vector<uint32_t> indices =
        {

            0,1,2,
            2,3,0,


            4,6,5,
            6,4,7,


            0,4,5,
            5,1,0,


            3,2,6,
            6,7,3,


            1,5,6,
            6,2,1,


            0,3,7,
            7,4,0

        };



        return Mesh(
            vertices,
            indices
        );


    }


    //Mesh Mesh::CreateTriangle()
    //{
    //    std::vector<Vertex> vertices =
    //    {
    //        // 0 左上
    //        {
    //            {-0.8f, 0.8f, 0.0f},
    //            {1.0f,0.0f,0.0f}
    //        },

    //        // 1 右上
    //        {
    //            {0.8f,0.8f,0.0f},
    //            {0.0f,1.0f,0.0f}
    //        },

    //        // 2 右下
    //        {
    //            {0.8f,-0.8f,0.0f},
    //            {0.0f,0.0f,1.0f}
    //        },

    //        // 3 左下
    //        {
    //            {-0.8f,-0.8f,0.0f},
    //            {1.0f,1.0f,1.0f}
    //        }
    //    };


    //    std::vector<uint32_t> indices =
    //    {
    //        0,1,2,   // 第一个三角形

    //        2,3,0    // 第二个三角形
    //    };


    //    return Mesh(
    //        vertices,
    //        indices
    //    );
    //}

}