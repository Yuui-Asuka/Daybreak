#pragma once

#include "Camera.h"

#include "Input/Input.h"


namespace Daybreak
{


    class CameraController
    {

    public:

        CameraController(
            Camera& camera
        );


        void Update(
            float deltaTime
        );


    private:

        Camera& m_Camera;


        bool m_FirstMouse = true;


        float m_LastMouseX = 0.0f;

        float m_LastMouseY = 0.0f;


    };


}