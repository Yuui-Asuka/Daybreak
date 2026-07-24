#include "Camera.h"


#include <glm/gtc/matrix_transform.hpp>

#include <cmath>


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


        m_Yaw =
            -90.0f;


        m_Pitch =
            0.0f;



        m_FOV =
            45.0f;


        m_Aspect =
            16.0f / 9.0f;


        m_Near =
            0.1f;


        m_Far =
            100.0f;



        m_Up =
        {
            0,
            1,
            0
        };


        UpdateVectors();

    }



    void Camera::UpdateVectors()
    {

        glm::vec3 front;


        front.x =
            cos(glm::radians(m_Yaw))
            *
            cos(glm::radians(m_Pitch));


        front.y =
            sin(glm::radians(m_Pitch));


        front.z =
            sin(glm::radians(m_Yaw))
            *
            cos(glm::radians(m_Pitch));


        m_Front =
            glm::normalize(front);



        m_Right =
            glm::normalize(
                glm::cross(
                    m_Front,
                    glm::vec3(0, 1, 0)
                )
            );



        m_Up =
            glm::normalize(
                glm::cross(
                    m_Right,
                    m_Front
                )
            );

    }



    glm::mat4 Camera::GetViewMatrix() const
    {

        return glm::lookAt(
            m_Position,
            m_Position + m_Front,
            m_Up
        );

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


        // Vulkan坐标修正

        projection[1][1] *= -1;


        return projection;

    }

    void Camera::ProcessKeyboard(
        int direction,
        float deltaTime
    )
    {

        float speed =
            5.0f * deltaTime;



        switch (direction)
        {

        case 0: // Forward

            m_Position +=
                m_Front * speed;

            break;



        case 1: // Backward

            m_Position -=
                m_Front * speed;

            break;



        case 2: // Left

            m_Position -=
                m_Right * speed;

            break;



        case 3: // Right

            m_Position +=
                m_Right * speed;

            break;

        }

    }

    void Camera::ProcessMouseMovement(
        float deltaX,
        float deltaY
    )
    {

        float sensitivity = 0.1f;


        deltaX *= sensitivity;
        deltaY *= sensitivity;



        m_Yaw += deltaX;


        m_Pitch -= deltaY;



        // 防止相机翻转

        if (m_Pitch > 89.0f)
        {
            m_Pitch = 89.0f;
        }


        if (m_Pitch < -89.0f)
        {
            m_Pitch = -89.0f;
        }



        UpdateVectors();

    }

    void Camera::SetAspectRatio(float aspect)
    {
        m_Aspect = aspect;
    }


    void Camera::SetPosition(
        const glm::vec3& position)
    {

        m_Position =
            position;

    }


}