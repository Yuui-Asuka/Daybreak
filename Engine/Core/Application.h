#pragma once


#include "../Window/Window.h"
#include "../Scene/Entity/Entity.h"
#include "../Scene/Scene/Scene.h"
#include "../Scene/Camera/Camera.h"
#include "../Scene/Camera/CameraController.h"
#include "Renderer/Mesh/Mesh.h"



class Application
{

public:

    Application();


    void RunVulkan();



private:


    Daybreak::Window m_Window;



    Daybreak::Camera m_Camera;


    Daybreak::CameraController
        m_CameraController;



    /*
        Scene

    */

    Daybreak::Scene m_Scene;



    /*
        Entity

    */

    Daybreak::Entity m_Cube;



    /*
        Mesh资源

    */

    Daybreak::Mesh m_CubeMesh;


};