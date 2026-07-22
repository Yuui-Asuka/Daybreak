#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Daybreak
{

    class Camera
    {

    public:

        Camera();


        glm::mat4 GetViewMatrix() const;


        glm::mat4 GetProjectionMatrix() const;



        void SetPosition(
            const glm::vec3& position
        );


        glm::vec3 GetPosition() const
        {
            return m_Position;
        }


    private:

        glm::vec3 m_Position;


        glm::vec3 m_Rotation;


        float m_FOV;


        float m_Aspect;


        float m_Near;


        float m_Far;

    };


}