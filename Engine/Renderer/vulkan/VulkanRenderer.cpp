#include "VulkanRenderer.h"
#include "../UBO/UniformBufferObject.h"
#include "Renderer/Mesh/Mesh.h"

#include <iostream>
#include <Scene/Entity/Entity.h>


namespace Daybreak
{


    /**
     * @brief Initializes the Vulkan renderer.
     *
     * Creates and initializes all rendering resources:
     *
     * - Vulkan context
     * - Physical and logical device
     * - Command pool
     * - Swapchain
     * - Render pass
     * - Shader modules
     * - Buffers
     * - Descriptor resources
     * - Graphics pipeline
     * - Frame synchronization objects
     *
     * @param window GLFW window used for rendering.
     */
    void VulkanRenderer::Init(
        GLFWwindow* window
    )
    {

        // Store the rendering window.
        m_Window = window;


        // Initialize Vulkan instance.
        m_Context.Init();


        // Create window surface.
        m_Context.CreateSurface(
            window
        );


        // Initialize GPU device.
        m_Device.Init(
            m_Context.GetInstance(),
            m_Context.GetSurface()
        );


        // Create command pool for GPU command allocation.
        m_CommandPool.Init(
            m_Device.GetDevice(),
            m_Device.GetGraphicsQueueFamily()
        );


        // Initialize texture resources.
        m_Texture.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            m_CommandPool.GetCommandPool(),
            m_Device.GetGraphicsQueue(),
            "D:/vs/projects/Daybreak/Daybreak/Assets/Texture/images.jpg"
        );


        // Create swapchain.
        m_Swapchain.Init(
            &m_Device,
            m_Context.GetSurface(),
            window
        );


        // Create render pass.
        m_RenderPass.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetImageFormat()
        );


        // Load shader modules.
        m_Shader.Init(
            m_Device.GetDevice(),
            "D:/vs/projects/Daybreak/Daybreak/Assets/Shaders/triangle.vert.spv",
            "D:/vs/projects/Daybreak/Daybreak/Assets/Shaders/triangle.frag.spv"
        );


        // Create cube mesh data.
        Mesh cube =
            Mesh::CreateCube();



        // Create vertex buffer.
        m_VertexBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            sizeof(Vertex)
            *
            cube.GetVertices().size(),
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
        );


        // Upload vertex data to GPU memory.
        m_VertexBuffer.Upload(
            cube.GetVertices().data(),
            sizeof(Vertex)
            *
            cube.GetVertices().size()
        );



        // Create index buffer.
        m_VulkanIndexBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            cube.GetIndices()
        );



        // Create uniform buffer.
        m_UniformBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            sizeof(UniformBufferObject)
        );



        // Create descriptor set layout.
        m_DescriptorSetLayout.Init(
            m_Device.GetDevice()
        );



        // Create descriptor pool.
        m_DescriptorPool.Init(
            m_Device.GetDevice()
        );



        // Retrieve texture descriptor information.
        VkDescriptorImageInfo textureInfo =
            m_Texture.GetDescriptorInfo();



        // Create descriptor set.
        m_DescriptorSet.Init(
            m_Device.GetDevice(),
            m_DescriptorPool.GetPool(),
            m_DescriptorSetLayout.GetLayout(),
            m_UniformBuffer.GetBuffer(),
            sizeof(UniformBufferObject),
            textureInfo
        );



        // Create graphics pipeline.
        m_Pipeline.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetExtent(),
            m_RenderPass.GetRenderPass(),
            m_Shader.GetVertexShader(),
            m_Shader.GetFragmentShader(),
            m_DescriptorSetLayout.GetLayout()
        );



        // Create depth buffer resources.
        m_DepthBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            m_Swapchain.GetExtent()
        );



        // Create framebuffer resources.
        m_Framebuffer.Init(
            m_Device.GetDevice(),
            m_RenderPass.GetRenderPass(),
            m_Swapchain.GetImageViews(),
            m_Swapchain.GetExtent(),
            m_DepthBuffer.GetImageView()
        );



        // Record rendering command buffers.
        m_CommandBuffer.Init(
            m_Device.GetDevice(),
            m_CommandPool.GetCommandPool(),
            m_RenderPass.GetRenderPass(),
            m_Swapchain.GetExtent(),
            m_Pipeline.GetPipeline(),
            m_Pipeline.GetPipelineLayout(),
            m_DescriptorSet.GetDescriptorSet(),
            m_Framebuffer.GetFramebuffers(),
            m_VertexBuffer.GetBuffer(),
            cube.GetVertexCount(),
            m_VulkanIndexBuffer.GetBuffer(),
            cube.GetIndexCount()
        );



        // Initialize camera parameters.
        if (m_Camera)
        {

            m_Camera->SetPosition(
                { 0,0,5 }
            );


            m_Camera->SetAspectRatio(
                float(m_Swapchain.GetExtent().width) /
                float(m_Swapchain.GetExtent().height)
            );


            m_Camera->ProcessMouseMovement(
                45,
                -20
            );

        }



        // Create synchronization objects.
        m_Sync.Init(
            m_Device.GetDevice()
        );

    }



    /**
 * @brief Renders one frame.
 *
 * Rendering process:
 *
 * 1. Wait for previous GPU work to finish.
 * 2. Update uniform buffer data.
 * 3. Acquire a swapchain image.
 * 4. Submit command buffer to GPU.
 * 5. Present rendered image to the window.
 */
    void VulkanRenderer::DrawFrame(
        Scene& scene
    )
    {

        for (Entity* entity :
            scene.GetEntities())
        {

            if (entity == nullptr)
                continue;

            for (Entity* entity :
                scene.GetEntities())
            {

                if (entity == nullptr)
                    continue;


                DrawEntity(
                    *entity
                );

            }
            // 更新Transform
            // 更新UBO
            // DrawIndexed

            //UniformBufferObject ubo{};



            //ubo.model =
            //    entity->Transform.GetMatrix();



            //ubo.view =
            //    m_Camera->GetViewMatrix();



            //ubo.projection =
            //    m_Camera->GetProjectionMatrix();



            //m_UniformBuffer.Upload(
            //    &ubo,
            //    sizeof(ubo)
            //);

            //if (m_Camera)
            //{

            //    ubo.view =
            //        m_Camera->GetViewMatrix();


            //    ubo.projection =
            //        m_Camera->GetProjectionMatrix();

            //}


            //// Upload transformation data to GPU.
            //m_UniformBuffer.Upload(
            //    &ubo,
            //    sizeof(ubo)
            //);
        }

        int width;
        int height;


        // Get current framebuffer size.
        glfwGetFramebufferSize(
            m_Window,
            &width,
            &height
        );


        // Skip rendering when the window is minimized.
        if (width == 0 || height == 0)
        {
            return;
        }



        // Wait until the previous frame has finished rendering.
        VkFence inFlightFence =
            m_Sync.GetInFlightFence();


        vkWaitForFences(
            m_Device.GetDevice(),
            1,
            &inFlightFence,
            VK_TRUE,
            UINT64_MAX
        );


        // Reset fence before submitting new GPU work.
        vkResetFences(
            m_Device.GetDevice(),
            1,
            &inFlightFence
        );



        // Update per-frame uniform data.
        //Daybreak::UniformBufferObject ubo{};


        //ubo.model =
        //    glm::mat4(1.0f);

        //ubo.model =
        //    entity.Transform.GetMatrix();


        


        // Acquire an available swapchain image.
        uint32_t imageIndex = 0;


        VkSemaphore imageAvailable =
            m_Sync.GetImageAvailableSemaphore();


        VkResult result =
            vkAcquireNextImageKHR(
                m_Device.GetDevice(),
                m_Swapchain.GetSwapchain(),
                UINT64_MAX,
                imageAvailable,
                VK_NULL_HANDLE,
                &imageIndex
            );


        // Recreate swapchain when the window size changes.
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapchain();
            return;
        }


        if (result != VK_SUCCESS &&
            result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error(
                "Failed to acquire swapchain image!"
            );
        }



        // Define the pipeline stage that waits for image availability.
        VkPipelineStageFlags waitStages[] =
        {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };



        // Semaphore signaled after rendering is completed.
        VkSemaphore renderFinished =
            m_Sync.GetRenderFinishedSemaphore();



        // Select command buffer matching the swapchain image.
        VkCommandBuffer commandBuffer =
            m_CommandBuffer.GetCommandBuffers()[imageIndex];



        // Configure GPU submission information.
        VkSubmitInfo submitInfo{};


        submitInfo.sType =
            VK_STRUCTURE_TYPE_SUBMIT_INFO;



        // Wait for swapchain image to become available.
        submitInfo.waitSemaphoreCount = 1;


        submitInfo.pWaitSemaphores =
            &imageAvailable;


        submitInfo.pWaitDstStageMask =
            waitStages;



        // Submit recorded rendering commands.
        submitInfo.commandBufferCount = 1;


        submitInfo.pCommandBuffers =
            &commandBuffer;



        // Signal semaphore after rendering finishes.
        submitInfo.signalSemaphoreCount = 1;


        submitInfo.pSignalSemaphores =
            &renderFinished;



        // Submit commands to graphics queue.
        VkResult resultx =
            vkQueueSubmit(
                m_Device.GetGraphicsQueue(),
                1,
                &submitInfo,
                inFlightFence
            );


        std::cout
            << "vkQueueSubmit result = "
            << resultx
            << std::endl;


        if (resultx != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to submit draw command!"
            );
        }



        // Prepare image presentation information.
        VkPresentInfoKHR presentInfo{};


        presentInfo.sType =
            VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;



        // Wait until rendering is finished before presenting.
        presentInfo.waitSemaphoreCount = 1;


        presentInfo.pWaitSemaphores =
            &renderFinished;



        VkSwapchainKHR swapchain =
            m_Swapchain.GetSwapchain();



        // Present one swapchain image.
        presentInfo.swapchainCount = 1;


        presentInfo.pSwapchains =
            &swapchain;



        // Specify image index to display.
        presentInfo.pImageIndices =
            &imageIndex;


        presentInfo.pResults =
            nullptr;



        // Submit presentation request.
        VkResult presentResult =
            vkQueuePresentKHR(
                m_Device.GetPresentQueue(),
                &presentInfo
            );


        // Recreate swapchain if it is invalid.
        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR ||
            presentResult == VK_SUBOPTIMAL_KHR)
        {
            RecreateSwapchain();
            return;
        }


        if (presentResult != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to present swapchain image!"
            );
        }

    }

    /**
 * @brief Releases all Vulkan renderer resources.
 *
 * Resources are destroyed in reverse order of creation
 * to respect Vulkan object dependencies.
 */
    void VulkanRenderer::Shutdown()
    {

        // Wait until all GPU operations are completed.
        vkDeviceWaitIdle(
            m_Device.GetDevice()
        );


        // Destroy synchronization resources.
        m_Sync.Shutdown();


        // Destroy command buffers.
        m_CommandBuffer.Shutdown();


        // Destroy framebuffer resources.
        m_Framebuffer.Shutdown();


        // Destroy depth buffer resources.
        m_DepthBuffer.Shutdown();


        // Destroy descriptor resources.
        m_DescriptorSet.Shutdown();

        m_DescriptorPool.Shutdown();

        m_DescriptorSetLayout.Shutdown();



        // Destroy GPU buffer resources.
        m_VulkanIndexBuffer.Shutdown();

        m_VertexBuffer.Shutdown();

        m_UniformBuffer.Shutdown();



        // Destroy graphics pipeline resources.
        m_Pipeline.Shutdown();



        // Destroy shader modules.
        m_Shader.Shutdown();



        // Destroy render pass.
        m_RenderPass.Shutdown();



        // Destroy swapchain resources.
        m_Swapchain.Shutdown();



        // Destroy texture resources.
        m_Texture.Shutdown();



        // Destroy command pool.
        m_CommandPool.Shutdown();



        // Destroy logical device.
        m_Device.Shutdown();



        // Destroy Vulkan instance and surface.
        m_Context.Shutdown();

    }


    /**
     * @brief Recreates swapchain-dependent resources.
     *
     * Called when:
     *
     * - Window size changes.
     * - Swapchain becomes invalid.
     *
     * Recreates:
     *
     * - Swapchain
     * - Render pass
     * - Pipeline
     * - Depth buffer
     * - Framebuffer
     * - Command buffers
     */
    void VulkanRenderer::RecreateSwapchain()
    {

        int width = 0;
        int height = 0;


        // Query current framebuffer size.
        glfwGetFramebufferSize(
            m_Window,
            &width,
            &height
        );



        // Wait until the window has a valid size.
        while (width == 0 || height == 0)
        {

            glfwGetFramebufferSize(
                m_Window,
                &width,
                &height
            );


            glfwWaitEvents();

        }



        // Ensure GPU is idle before recreating resources.
        vkDeviceWaitIdle(
            m_Device.GetDevice()
        );



        /*
            Destroy old swapchain-dependent resources.
        */

        m_CommandBuffer.Shutdown();

        m_Framebuffer.Shutdown();

        m_DepthBuffer.Shutdown();

        m_Pipeline.Shutdown();

        m_RenderPass.Shutdown();

        m_Swapchain.Shutdown();



        /*
            Create new swapchain.
        */

        m_Swapchain.Init(
            &m_Device,
            m_Context.GetSurface(),
            m_Window
        );


        std::cout
            << "New extent: "
            << m_Swapchain.GetExtent().width
            << " x "
            << m_Swapchain.GetExtent().height
            << std::endl;



        /*
            Recreate render pass.
        */

        m_RenderPass.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetImageFormat()
        );



        /*
            Recreate graphics pipeline.
        */

        m_Pipeline.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetExtent(),
            m_RenderPass.GetRenderPass(),
            m_Shader.GetVertexShader(),
            m_Shader.GetFragmentShader(),
            m_DescriptorSetLayout.GetLayout()
        );



        /*
            Recreate depth resources.
        */

        m_DepthBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            m_Swapchain.GetExtent()
        );



        /*
            Recreate framebuffers.
        */

        m_Framebuffer.Init(
            m_Device.GetDevice(),
            m_RenderPass.GetRenderPass(),
            m_Swapchain.GetImageViews(),
            m_Swapchain.GetExtent(),
            m_DepthBuffer.GetImageView()
        );



        /*
            Re-record command buffers.
        */

        Daybreak::Mesh cube =
            Daybreak::Mesh::CreateCube();



        m_CommandBuffer.Init(
            m_Device.GetDevice(),
            m_CommandPool.GetCommandPool(),
            m_RenderPass.GetRenderPass(),
            m_Swapchain.GetExtent(),
            m_Pipeline.GetPipeline(),
            m_Pipeline.GetPipelineLayout(),
            m_DescriptorSet.GetDescriptorSet(),
            m_Framebuffer.GetFramebuffers(),

            m_VertexBuffer.GetBuffer(),
            cube.GetVertexCount(),

            m_VulkanIndexBuffer.GetBuffer(),
            cube.GetIndexCount()
        );



        // Update camera projection after resize.
        if (m_Camera)
        {

            m_Camera->SetAspectRatio(
                float(m_Swapchain.GetExtent().width) /
                float(m_Swapchain.GetExtent().height)
            );

        }

    }





    /**
     * @brief Sets the active rendering camera.
     *
     * The camera provides:
     *
     * - View matrix
     * - Projection matrix
     *
     * These matrices are uploaded to shaders through
     * the uniform buffer.
     *
     * @param camera Camera instance used for rendering.
     */
    void Daybreak::VulkanRenderer::SetCamera(
        Camera* camera
    )
    {
        m_Camera = camera;
    }


    void VulkanRenderer::DrawEntity(
        Entity& entity
    )
    {

        /*
            更新当前Entity的Transform

            model:
                当前物体矩阵

            view:
                Camera矩阵

            projection:
                投影矩阵

        */

        UniformBufferObject ubo{};


        ubo.model =
            entity.Transform.GetMatrix();



        if (m_Camera)
        {

            ubo.view =
                m_Camera->GetViewMatrix();


            ubo.projection =
                m_Camera->GetProjectionMatrix();

        }



        /*
            上传UBO

        */

        m_UniformBuffer.Upload(
            &ubo,
            sizeof(ubo)
        );



        /*
            把绘制请求交给CommandBuffer

        */

        //m_CommandBuffer.DrawMesh(
        //    entity.Mesh
        //);

    }

}