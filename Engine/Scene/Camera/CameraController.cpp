#include "CameraController.h"

#include <GLFW/glfw3.h>


namespace Daybreak
{


    CameraController::CameraController(
        Camera& camera
    )
        :
        m_Camera(camera)
    {

    }



    void CameraController::Update(
        float deltaTime
    )
    {

        /*
            WASD移动
        */

        if (Input::IsKeyPressed(GLFW_KEY_W))
        {
            m_Camera.ProcessKeyboard(
                0,
                deltaTime
            );
        }


        if (Input::IsKeyPressed(GLFW_KEY_S))
        {
            m_Camera.ProcessKeyboard(
                1,
                deltaTime
            );
        }


        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            m_Camera.ProcessKeyboard(
                2,
                deltaTime
            );
        }


        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            m_Camera.ProcessKeyboard(
                3,
                deltaTime
            );
        }



        /*
            鼠标旋转
        */


        if (Input::IsMouseButtonPressed(
            GLFW_MOUSE_BUTTON_RIGHT))
        {

            glm::vec2 mouse =
                Input::GetMousePosition();



            if (m_FirstMouse)
            {
                m_LastMouseX = mouse.x;
                m_LastMouseY = mouse.y;

                m_FirstMouse = false;
            }



            float deltaX =
                mouse.x - m_LastMouseX;


            float deltaY =
                mouse.y - m_LastMouseY;



            m_LastMouseX = mouse.x;
            m_LastMouseY = mouse.y;



            m_Camera.ProcessMouseMovement(
                deltaX,
                deltaY
            );

        }
        else
        {
            m_FirstMouse = true;
        }


    }


}