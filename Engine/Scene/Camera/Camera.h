#pragma once

#include <glm/glm.hpp>


namespace Daybreak
{


    class Camera
    {

    public:

        Camera();


        glm::mat4 GetViewMatrix() const;

        glm::mat4 GetProjectionMatrix() const;

        void SetAspectRatio(
            float aspect
        );

        void SetPosition(
            const glm::vec3& position
        );


        void ProcessMouseMovement(
            float deltaX,
            float deltaY
        );


        void ProcessKeyboard(
            int direction,
            float deltaTime
        );


    private:


        glm::vec3 m_Position;


        glm::vec3 m_Front;


        glm::vec3 m_Up;


        glm::vec3 m_Right;



        float m_Yaw;


        float m_Pitch;



        float m_FOV;


        float m_Aspect;


        float m_Near;


        float m_Far;



    private:


        void UpdateVectors();

    };


}