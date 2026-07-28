#include "Renderer.h"


namespace Daybreak
{

    /**
     * @brief Global Vulkan renderer instance.
     *
     * Renderer provides a static access layer while the actual
     * rendering implementation is handled by VulkanRenderer.
     */
    VulkanRenderer* Renderer::s_Renderer = nullptr;



    /**
     * @brief Initializes the rendering system.
     *
     * Creates the internal Vulkan renderer instance and initializes
     * all rendering resources associated with the provided window.
     *
     * @param window GLFW window handle used for rendering.
     */
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



    /**
     * @brief Shuts down the rendering system.
     *
     * Releases all Vulkan renderer resources and destroys the
     * internal renderer instance.
     */
    void Renderer::Shutdown()
    {

        if (s_Renderer)
        {

            s_Renderer->Shutdown();


            delete s_Renderer;


            s_Renderer = nullptr;

        }

    }



    /**
     * @brief Renders one frame.
     *
     * Delegates frame rendering to the Vulkan renderer.
     */
    void Renderer::DrawFrame(
        Scene& scene
    )
    {
        s_Renderer->DrawFrame(
            scene
        );
    }



    /**
     * @brief Sets the active camera used for rendering.
     *
     * The camera data is forwarded to the Vulkan renderer,
     * which uses it when updating rendering transformations.
     *
     * @param camera Camera instance used for view/projection data.
     */
    void Renderer::SetCamera(
        Camera* camera
    )
    {
        s_Renderer->SetCamera(camera);
    }


    void Renderer::Render(
        Scene& scene
    )
    {

        const auto& entities =
            scene.GetEntities();


        for (Entity* entity : entities)
        {

            if (entity == nullptr)
                continue;


            /*
                这里以后调用 VulkanRenderer

                例如:

                VulkanRenderer::DrawEntity(entity)

            */


        }

    }


}