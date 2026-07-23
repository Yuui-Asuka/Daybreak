#include "Input.h"


#include <GLFW/glfw3.h>



namespace Daybreak
{


    GLFWwindow* Input::s_Window = nullptr;



    void Input::Init(
        GLFWwindow* window
    )
    {
        s_Window = window;
    }



    bool Input::IsKeyPressed(
        int key
    )
    {

        return glfwGetKey(
            s_Window,
            key
        )
            ==
            GLFW_PRESS;

    }



    bool Input::IsMouseButtonPressed(
        int button
    )
    {

        return glfwGetMouseButton(
            s_Window,
            button
        )
            ==
            GLFW_PRESS;

    }



    glm::vec2 Input::GetMousePosition()
    {

        double x;
        double y;


        glfwGetCursorPos(
            s_Window,
            &x,
            &y
        );


        return {
            (float)x,
            (float)y
        };

    }


}