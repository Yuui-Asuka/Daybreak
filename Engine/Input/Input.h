#pragma once

#include <glm/glm.hpp>


struct GLFWwindow;


namespace Daybreak
{


    class Input
    {

    public:


        static void Init(
            GLFWwindow* window
        );


        static bool IsKeyPressed(
            int key
        );


        static bool IsMouseButtonPressed(
            int button
        );


        static glm::vec2 GetMousePosition();



    private:

        static GLFWwindow* s_Window;


    };


}