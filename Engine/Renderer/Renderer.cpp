#include "Renderer.h"


namespace Daybreak
{


    VulkanRenderer* Renderer::s_Renderer = nullptr;



    void Renderer::Init(
        GLFWwindow* window
    )
    {

        s_Renderer =
            new VulkanRenderer();


        s_Renderer->Init(
            window
        );

    }



    void Renderer::Shutdown()
    {

        if (s_Renderer)
        {

            s_Renderer->Shutdown();


            delete s_Renderer;


            s_Renderer = nullptr;

        }

    }



    void Renderer::DrawFrame()
    {
        s_Renderer->DrawFrame();
    }

    void Daybreak::Renderer::SetCamera(
        Camera* camera
    )
    {
        s_Renderer->SetCamera(camera);
    }

}