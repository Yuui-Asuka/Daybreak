#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Daybreak
{

    glm::mat4 Transform::GetMatrix() const
    {

        glm::mat4 model(1.0f);


        // Translation
        model =
            glm::translate(
                model,
                Position
            );


        // Rotation X
        model =
            glm::rotate(
                model,
                glm::radians(Rotation.x),
                glm::vec3(1.0f, 0.0f, 0.0f)
            );


        // Rotation Y
        model =
            glm::rotate(
                model,
                glm::radians(Rotation.y),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );


        // Rotation Z
        model =
            glm::rotate(
                model,
                glm::radians(Rotation.z),
                glm::vec3(0.0f, 0.0f, 1.0f)
            );


        // Scale
        model =
            glm::scale(
                model,
                Scale
            );


        return model;

    }

}