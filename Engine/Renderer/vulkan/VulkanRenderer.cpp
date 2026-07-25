#include "VulkanRenderer.h"
#include "../UBO/UniformBufferObject.h"
#include "Renderer/Mesh/Mesh.h"

#include <iostream>



namespace Daybreak
{


    void VulkanRenderer::Init(
        GLFWwindow* window
    )
    {

        m_Window = window;



        m_Context.Init();


        m_Context.CreateSurface(
            window
        );



        m_Device.Init(
            m_Context.GetInstance(),
            m_Context.GetSurface()
        );



        m_CommandPool.Init(
            m_Device.GetDevice(),
            m_Device.GetGraphicsQueueFamily()
        );



        m_Texture.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            m_CommandPool.GetCommandPool(),
            m_Device.GetGraphicsQueue(),
            "D:/vs/projects/Daybreak/Daybreak/Assets/Texture/images.jpg"
        );



        m_Swapchain.Init(
            &m_Device,
            m_Context.GetSurface(),
            window
        );



        m_RenderPass.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetImageFormat()
        );



        m_Shader.Init(
            m_Device.GetDevice(),
            "D:/vs/projects/Daybreak/Daybreak/Assets/Shaders/triangle.vert.spv",
            "D:/vs/projects/Daybreak/Daybreak/Assets/Shaders/triangle.frag.spv"
        );



        Mesh cube =
            Mesh::CreateCube();



        m_VertexBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            sizeof(Vertex)
            *
            cube.GetVertices().size(),
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
        );



        m_VertexBuffer.Upload(
            cube.GetVertices().data(),
            sizeof(Vertex)
            *
            cube.GetVertices().size()
        );



        m_VulkanIndexBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            cube.GetIndices()
        );



        m_UniformBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            sizeof(UniformBufferObject)
        );



        m_DescriptorSetLayout.Init(
            m_Device.GetDevice()
        );



        m_DescriptorPool.Init(
            m_Device.GetDevice()
        );



        VkDescriptorImageInfo textureInfo =
            m_Texture.GetDescriptorInfo();



        m_DescriptorSet.Init(
            m_Device.GetDevice(),
            m_DescriptorPool.GetPool(),
            m_DescriptorSetLayout.GetLayout(),
            m_UniformBuffer.GetBuffer(),
            sizeof(UniformBufferObject),
            textureInfo
        );



        m_Pipeline.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetExtent(),
            m_RenderPass.GetRenderPass(),
            m_Shader.GetVertexShader(),
            m_Shader.GetFragmentShader(),
            m_DescriptorSetLayout.GetLayout()
        );



        m_DepthBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            m_Swapchain.GetExtent()
        );



        m_Framebuffer.Init(
            m_Device.GetDevice(),
            m_RenderPass.GetRenderPass(),
            m_Swapchain.GetImageViews(),
            m_Swapchain.GetExtent(),
            m_DepthBuffer.GetImageView()
        );



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

        if (m_Camera) {

            m_Camera->SetPosition({ 0,0,5 });

            m_Camera->SetAspectRatio(
                float(m_Swapchain.GetExtent().width) /
                float(m_Swapchain.GetExtent().height)
            );

            m_Camera->ProcessMouseMovement(
                45,
                -20
            );

        }
        m_Sync.Init(
            m_Device.GetDevice()
        );


    }

    void VulkanRenderer::DrawFrame()
    {
        int width;
        int height;

        glfwGetFramebufferSize(
            m_Window,
            &width,
            &height
        );


        if (width == 0 || height == 0)
        {
            return;
        }



        /*
            等待上一帧GPU执行完成。

            Fence用于CPU和GPU同步。

            如果GPU还没有完成上一帧：

                CPU会阻塞等待。

            防止CPU提交过快导致资源冲突。
        */
        VkFence inFlightFence =
            m_Sync.GetInFlightFence();

        vkWaitForFences(
            m_Device.GetDevice(),
            1,
            &inFlightFence,
            VK_TRUE,
            UINT64_MAX
        );


        /*
            重置Fence。

            vkQueueSubmit提交任务时：

                Fence状态会变为未完成。

            下一帧再次等待。
        */
        vkResetFences(
            m_Device.GetDevice(),
            1,
            &inFlightFence
        );


        // ==========================
       // 更新 Uniform Buffer
       // ==========================

        Daybreak::UniformBufferObject ubo{};


        ubo.model =
            glm::mat4(1.0f);
        if (m_Camera) {

            ubo.view =
                m_Camera->GetViewMatrix();


            ubo.projection =
                m_Camera->GetProjectionMatrix();

        }

        m_UniformBuffer.Upload(
            &ubo,
            sizeof(ubo)
        );


        m_UniformBuffer.Upload(
            &ubo,
            sizeof(ubo)
        );

        /*
            从Swapchain获取一张可绘制Image。

            imageAvailable Semaphore:

                当Image准备完成后触发。


            imageIndex:

                当前需要渲染的Swapchain Image编号。

        */
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

        //std::cout << "Acquire Done\n";
        /*
            指定GPU执行CommandBuffer之前：

            需要等待的Pipeline阶段。


            当前：

                等待Color Attachment输出阶段。

        */
        VkPipelineStageFlags waitStages[] =
        {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };


        /*
            GPU完成渲染后：

                触发Render Finished Semaphore。


            Present Queue等待这个信号。

        */
        VkSemaphore renderFinished =
            m_Sync.GetRenderFinishedSemaphore();


        /*
            根据Swapchain Image选择对应CommandBuffer。


            关系：

                Swapchain Image

                        |

                        v

                  Framebuffer

                        |

                        v

                  CommandBuffer

        */
        VkCommandBuffer commandBuffer =
            m_CommandBuffer.GetCommandBuffers()[imageIndex];


        /*
            Queue Submit信息。

            描述：

                等待什么Semaphore

                执行什么CommandBuffer

                完成后触发什么Semaphore

        */
        VkSubmitInfo submitInfo{};

        submitInfo.sType =
            VK_STRUCTURE_TYPE_SUBMIT_INFO;


        /*
            等待Image Available Semaphore。

            确保Swapchain Image已经可以写入。
        */
        submitInfo.waitSemaphoreCount = 1;

        submitInfo.pWaitSemaphores =
            &imageAvailable;


        submitInfo.pWaitDstStageMask =
            waitStages;


        /*
            提交CommandBuffer。

            GPU执行这里记录的：

                BeginRenderPass

                BindPipeline

                Draw

                EndRenderPass

        */
        submitInfo.commandBufferCount = 1;

        submitInfo.pCommandBuffers =
            &commandBuffer;


        /*
            GPU完成绘制后：

                触发Render Finished Semaphore。

            供Present使用。
        */
        submitInfo.signalSemaphoreCount = 1;

        submitInfo.pSignalSemaphores =
            &renderFinished;


        /*
            提交绘制任务到Graphics Queue。


            Fence:

                GPU完成后自动Signaled。


            下一帧vkWaitForFences等待它。

        */

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


        //if (vkQueueSubmit(
        //    m_Device.GetGraphicsQueue(),
        //    1,
        //    &submitInfo,
        //    inFlightFence)
        //    != VK_SUCCESS)
        //{
        //    throw std::runtime_error(
        //        "Failed to submit draw command!"
        //    );
        //}
        //std::cout << "Submit Done\n";

        /*
            Present信息。

            告诉Present Queue：

                显示哪一个Swapchain Image。

        */
        VkPresentInfoKHR presentInfo{};

        presentInfo.sType =
            VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;


        /*
            Present之前等待GPU渲染完成。

        */
        presentInfo.waitSemaphoreCount = 1;

        presentInfo.pWaitSemaphores =
            &renderFinished;


        VkSwapchainKHR swapchain =
            m_Swapchain.GetSwapchain();


        /*
            当前只有一个Swapchain。

        */
        presentInfo.swapchainCount = 1;

        presentInfo.pSwapchains =
            &swapchain;


        /*
            指定显示的Image。

        */
        presentInfo.pImageIndices =
            &imageIndex;


        presentInfo.pResults =
            nullptr;

        VkResult presentResult =
            vkQueuePresentKHR(
                m_Device.GetPresentQueue(),
                &presentInfo
            );


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

    void VulkanRenderer::Shutdown()
    {

        vkDeviceWaitIdle(
            m_Device.GetDevice()
        );


        m_Sync.Shutdown();

        m_CommandBuffer.Shutdown();

        m_Framebuffer.Shutdown();

        m_DepthBuffer.Shutdown();

        m_DescriptorSet.Shutdown();

        m_DescriptorPool.Shutdown();

        m_DescriptorSetLayout.Shutdown();

        m_VulkanIndexBuffer.Shutdown();

        m_VertexBuffer.Shutdown();

        m_UniformBuffer.Shutdown();

        m_Pipeline.Shutdown();

        m_Shader.Shutdown();

        m_RenderPass.Shutdown();

        m_Swapchain.Shutdown();

        m_Texture.Shutdown();

        m_CommandPool.Shutdown();

        m_Device.Shutdown();

        m_Context.Shutdown();

    }

    void VulkanRenderer::RecreateSwapchain()
    {

        int width = 0;
        int height = 0;


        glfwGetFramebufferSize(
            m_Window,
            &width,
            &height
        );


        // 最小化窗口时等待
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(
                m_Window,
                &width,
                &height
            );

            glfwWaitEvents();
        }



        vkDeviceWaitIdle(
            m_Device.GetDevice()
        );



        /*
            1. 销毁旧资源
        */


        m_CommandBuffer.Shutdown();

        m_Framebuffer.Shutdown();

        m_DepthBuffer.Shutdown();

        m_Pipeline.Shutdown();

        m_RenderPass.Shutdown();

        m_Swapchain.Shutdown();



        /*
            2. 创建新的 Swapchain
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
            3. RenderPass
        */


        m_RenderPass.Shutdown();


        m_RenderPass.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetImageFormat()
        );

        m_Pipeline.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetExtent(),
            m_RenderPass.GetRenderPass(),
            m_Shader.GetVertexShader(),
            m_Shader.GetFragmentShader(),
            m_DescriptorSetLayout.GetLayout()
        );



        /*
            4. Depth Buffer
        */


        m_DepthBuffer.Init(
            m_Device.GetDevice(),
            m_Device.GetPhysicalDevice(),
            m_Swapchain.GetExtent()
        );



        /*
            5. Framebuffer
        */


        m_Framebuffer.Init(
            m_Device.GetDevice(),
            m_RenderPass.GetRenderPass(),
            m_Swapchain.GetImageViews(),
            m_Swapchain.GetExtent(),
            m_DepthBuffer.GetImageView()
        );



        /*
            6. CommandBuffer重新录制
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

        if (m_Camera) {
            m_Camera->SetAspectRatio(
                float(m_Swapchain.GetExtent().width) /
                float(m_Swapchain.GetExtent().height)
            );
        }
    }


    void Daybreak::VulkanRenderer::SetCamera(
        Camera* camera
    )
    {
        m_Camera = camera;
    }

}