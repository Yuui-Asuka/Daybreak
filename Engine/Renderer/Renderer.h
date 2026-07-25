#pragma once


#include "Vulkan/VulkanRenderer.h"


namespace Daybreak
{


    class Renderer
    {

    public:

        static void Init(GLFWwindow* window);


        static void Shutdown();


        static void DrawFrame();

        static void SetCamera(
            Camera* camera
        );

    private:

        static VulkanRenderer* s_Renderer;


    };


}