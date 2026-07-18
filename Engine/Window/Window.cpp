#include "Window.h"

#include <stdexcept>
#include <iostream>


namespace Daybreak
{


    Window::Window(
        uint32_t width,
        uint32_t height,
        const char* title
    )
    {

        if (!glfwInit())
        {
            throw std::runtime_error(
                "Failed to init GLFW"
            );
        }


        glfwWindowHint(
            GLFW_CLIENT_API,
            GLFW_NO_API
        );


        m_Window =
            glfwCreateWindow(
                width,
                height,
                title,
                nullptr,
                nullptr
            );


        if (!m_Window)
        {
            throw std::runtime_error(
                "Failed to create window"
            );
        }


        std::cout
            << "GLFW Window Created"
            << std::endl;

    }



    Window::~Window()
    {

        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
        }


        glfwTerminate();

    }


}