#include "Application.h"

#include "Renderer/Renderer.h"


#include "Scene/Entity/Entity.h"
#include "Scene/Scene/Scene.h"

#include "Input/Input.h"

#include <GLFW/glfw3.h>

#include <iostream>



Application::Application()
    :
    m_Window(
        1280,
        720,
        "Daybreak"
    ),

    m_CameraController(
        m_Camera
    ),

    m_CubeMesh(
        Daybreak::Mesh::CreateCube()
    )

{

    Daybreak::Input::Init(
        m_Window.GetNativeWindow()
    );


}



void Application::RunVulkan()
{


    /*
        初始化Renderer。

        Renderer内部负责：

            VulkanContext

            Device

            Swapchain

            Pipeline

            Buffer

            CommandBuffer

    */

    Daybreak::Renderer::Init(
        m_Window.GetNativeWindow()
    );



    /*
        Camera交给Renderer。

        Renderer更新UBO时使用。

    */

    Daybreak::Renderer::SetCamera(
        &m_Camera
    );



    /*
        创建场景对象。

        Application只负责创建Scene。

    */


    m_Cube.Mesh =
        &m_CubeMesh;



    m_Cube.Transform.Position =
    {
        0.0f,
        0.0f,
        0.0f
    };



    m_Scene.AddEntity(
        &m_Cube
    );



    /*
        设置Camera。

    */

    m_Camera.SetPosition(
        {
            0,
            0,
            5
        }
    );



    float deltaTime =
        0.001f;



    while (
        !glfwWindowShouldClose(
            m_Window.GetNativeWindow()
        )
        )
    {


        glfwPollEvents();



        /*
            更新输入。

        */

        m_CameraController.Update(
            deltaTime
        );



        /*
            Renderer渲染Scene。

        */

        Daybreak::Renderer::DrawFrame(
            m_Scene
        );


    }



    Daybreak::Renderer::Shutdown();


}