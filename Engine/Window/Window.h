#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace Daybreak
{

    class Window
    {

    public:

        Window(
            uint32_t width,
            uint32_t height,
            const char* title
        );


        ~Window();


        GLFWwindow* GetNativeWindow()
        {
            return m_Window;
        }


    private:

        GLFWwindow* m_Window = nullptr;

    };

}