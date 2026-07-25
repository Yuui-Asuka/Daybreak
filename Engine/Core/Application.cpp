#include "Application.h"
#include "Renderer/Renderer.h"
#include "../Renderer/Vulkan/VulkanContext.h"
#include "Renderer/Mesh/Vertex.h"
#include "../Renderer/UBO/UniformBufferObject.h"
#include "Input/Input.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



Application::Application()
    :
    m_Window(1280, 720, "Daybreak"),
    m_CameraController(m_Camera)

{
    Daybreak::Input::Init(
        m_Window.GetNativeWindow()
    );
}

void Application::RunVulkan()
{

   

    Daybreak::Renderer::Init(
        m_Window.GetNativeWindow()
    );


    Daybreak::Renderer::SetCamera(
        &m_Camera
    );

    float deltaTime = 0.001f;


    while (
        !glfwWindowShouldClose(
            m_Window.GetNativeWindow()
        )
        )
    {

        glfwPollEvents();


        m_CameraController.Update(
            deltaTime
        );


        Daybreak::Renderer::DrawFrame();

    }


    Daybreak::Renderer::Shutdown();

}



