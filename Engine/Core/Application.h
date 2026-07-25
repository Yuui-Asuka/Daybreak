#pragma once


#include "../Window/Window.h"

#include "../Scene/Camera/Camera.h"
#include "../Scene/Camera/CameraController.h"



class Application
{

public:


    Application();


    void Run();

    void RunVulkan();


private:



private:


    Daybreak::Window m_Window;



    Daybreak::Camera m_Camera;


    Daybreak::CameraController m_CameraController;


};