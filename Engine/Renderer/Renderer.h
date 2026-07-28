#pragma once


#include "Vulkan/VulkanRenderer.h"
#include <Scene/Scene/Scene.h>


namespace Daybreak
{

    /**
     * @brief High-level rendering interface.
     *
     * Renderer provides a global access point for the engine rendering
     * system while hiding the underlying graphics API implementation.
     *
     * The actual rendering work is performed by VulkanRenderer.
     *
     * Responsibilities:
     *
     * - Initialize the renderer backend.
     * - Submit frame rendering requests.
     * - Manage renderer shutdown.
     * - Forward rendering-related data such as camera information.
     */
    class Renderer
    {

    public:

        /**
         * @brief Initializes the rendering backend.
         *
         * Creates the internal Vulkan renderer instance.
         *
         * @param window GLFW window handle used by the renderer.
         */
        static void Init(
            GLFWwindow* window
        );


        /**
         * @brief Shuts down the rendering backend.
         *
         * Releases all renderer resources.
         */
        static void Shutdown();


        /**
         * @brief Renders a single frame.
         *
         * Delegates frame execution to the active renderer backend.
         */
        static void DrawFrame(
            Scene& scene
        );

        /**
         * @brief Sets the active rendering camera.
         *
         * The renderer does not take ownership of the camera object.
         *
         * @param camera Camera instance used for view and projection data.
         */
        static void SetCamera(
            Camera* camera
        );

        static void Render(
            Scene& scene
        );



    private:

        /**
         * @brief Active renderer backend instance.
         *
         * Owned by Renderer and destroyed during Shutdown().
         */
        static VulkanRenderer* s_Renderer;


    };

}