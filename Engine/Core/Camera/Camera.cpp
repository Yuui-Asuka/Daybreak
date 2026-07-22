#include "Camera.h"


namespace Daybreak
{


    Camera::Camera()
    {

        m_Position =
        {
            0.0f,
            0.0f,
            3.0f
        };


        m_Rotation =
        {
            0.0f,
            0.0f,
            0.0f
        };


        m_FOV =
            45.0f;


        m_Aspect =
            1280.0f /
            720.0f;


        m_Near =
            0.1f;


        m_Far =
            100.0f;

    }



    glm::mat4 Camera::GetViewMatrix() const
    {

        glm::mat4 rotation =
            glm::mat4(1.0f);



        rotation =
            glm::rotate(
                rotation,
                glm::radians(m_Rotation.x),
                glm::vec3(1, 0, 0)
            );


        rotation =
            glm::rotate(
                rotation,
                glm::radians(m_Rotation.y),
                glm::vec3(0, 1, 0)
            );



        glm::mat4 translation =
            glm::translate(
                glm::mat4(1.0f),
                -m_Position
            );



        return rotation * translation;

    }



    glm::mat4 Camera::GetProjectionMatrix() const
    {

        glm::mat4 projection =
            glm::perspective(
                glm::radians(m_FOV),
                m_Aspect,
                m_Near,
                m_Far
            );


        // Vulkan Y轴需要翻转
        projection[1][1] *= -1;


        return projection;

    }



    void Camera::SetPosition(
        const glm::vec3& position
    )
    {
        m_Position = position;
    }



}