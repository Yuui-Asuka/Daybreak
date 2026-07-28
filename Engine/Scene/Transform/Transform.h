#pragma once

#include <glm/glm.hpp>

namespace Daybreak
{

    class Transform
    {

    public:

        Transform() = default;

        glm::vec3 Position =
        {
            0.0f,
            0.0f,
            0.0f
        };

        glm::vec3 Rotation =
        {
            0.0f,
            0.0f,
            0.0f
        };

        glm::vec3 Scale =
        {
            1.0f,
            1.0f,
            1.0f
        };

        glm::mat4 GetMatrix() const;

    };

}