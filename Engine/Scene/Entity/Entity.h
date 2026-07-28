#pragma once

#include "../Transform/Transform.h"


namespace Daybreak
{

    class Mesh;
    class VulkanTexture;


    class Entity
    {

    public:

        Entity() = default;


    public:

        /*
            每个Entity都有自己的Transform。


            例如：

            Cube1:

                Position = (0,0,0)


            Cube2:

                Position = (5,0,0)


            但是共享同一个Mesh。
        */

        Transform Transform;



        /*
            Mesh资源。


            多个Entity可以共享一个Mesh。

            例如：

                Tree1
                Tree2
                Tree3


            都可以指向：

                TreeMesh

        */

        Mesh* Mesh = nullptr;



        /*
            Texture资源。


            多个Entity可以共享纹理。

        */

        VulkanTexture* Texture = nullptr;


    };


}