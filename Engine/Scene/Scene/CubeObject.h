#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Renderer/Mesh/Mesh.h"



namespace Daybreak
{


    class GameObject
    {

    public:


        Mesh* mesh;


        glm::vec3 position =
        {
            0,0,0
        };


        glm::vec3 rotation =
        {
            0,0,0
        };


        glm::vec3 scale =
        {
            1,1,1
        };



        glm::mat4 GetModelMatrix()
        {

            glm::mat4 model =
                glm::mat4(1.0f);



            model =
                glm::translate(
                    model,
                    position
                );


            model =
                glm::rotate(
                    model,
                    rotation.x,
                    { 1,0,0 }
                );


            model =
                glm::rotate(
                    model,
                    rotation.y,
                    { 0,1,0 }
                );


            model =
                glm::scale(
                    model,
                    scale
                );


            return model;

        }


    };


}