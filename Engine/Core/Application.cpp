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
    // 1. 创建 Vulkan Instance
    m_Context.Init();


    // 2. 创建 Surface
    m_Context.CreateSurface(
        m_Window.GetNativeWindow()
    );


    // 3. 创建 Physical Device + Logical Device
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
        "D:/vs/projects/Daybreak/Daybreak/Assets/Texture/test1.jpg"
    );


    // 4. 创建 Swapchain
    m_Swapchain.Init(
        &m_Device,
        m_Context.GetSurface(),
        m_Window.GetNativeWindow()
    );

    std::cout
        << "New extent: "
        << m_Swapchain.GetExtent().width
        << " x "
        << m_Swapchain.GetExtent().height
        << std::endl;


    // 5. 创建 RenderPass
    m_RenderPass.Init(
        m_Device.GetDevice(),
        m_Swapchain.GetImageFormat()
    );


    // 6. 创建 Shader Module
    m_Shader.Init(
        m_Device.GetDevice(),
        "D:/vs/projects/Daybreak/Daybreak/Assets/Shaders/triangle.vert.spv",
        "D:/vs/projects/Daybreak/Daybreak/Assets/Shaders/triangle.frag.spv"
    );

    // Mesh
    Daybreak::Mesh triangle =
        Daybreak::Mesh::CreateCube();


    m_VertexBuffer.Init(
        m_Device.GetDevice(),
        m_Device.GetPhysicalDevice(),
        sizeof(Daybreak::Vertex) * triangle.GetVertices().size(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    );


    m_VertexBuffer.Upload(
        triangle.GetVertices().data(),
        sizeof(Daybreak::Vertex) * triangle.GetVertices().size()
    );

    m_UniformBuffer.Init(
        m_Device.GetDevice(),
        m_Device.GetPhysicalDevice(),
        sizeof(Daybreak::UniformBufferObject)
    );

    m_VulkanIndexBuffer.Init(
        m_Device.GetDevice(),
        m_Device.GetPhysicalDevice(),
        triangle.GetIndices()
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
        sizeof(Daybreak::UniformBufferObject),
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
    triangle.GetVertexCount(),
    m_VulkanIndexBuffer.GetBuffer(),
    triangle.GetIndexCount()
);

    m_Camera.SetPosition({ 0,0,5 });

    m_Camera.SetAspectRatio(
        float(m_Swapchain.GetExtent().width) /
        float(m_Swapchain.GetExtent().height)
    );

    m_Camera.ProcessMouseMovement(
        45,
        -20
    );

    m_Sync.Init(
        m_Device.GetDevice()
    );
    float deltaTime = 0.001f;

    while (!glfwWindowShouldClose(m_Window.GetNativeWindow()))
    {
        glfwPollEvents();

        m_CameraController.Update(
            deltaTime
        );

        DrawFrame();
    }

    m_Sync.Shutdown();

    m_CommandBuffer.Shutdown();

    m_CommandPool.Shutdown();

    m_VulkanIndexBuffer.Shutdown();

    m_VertexBuffer.Shutdown();

    m_UniformBuffer.Shutdown();

    m_Framebuffer.Shutdown();

    m_DescriptorPool.Shutdown();

    m_DescriptorSetLayout.Shutdown();

    m_DescriptorSet.Shutdown();

    m_Pipeline.Shutdown();

    m_Shader.Shutdown();

    m_RenderPass.Shutdown();

    m_Swapchain.Shutdown();

    m_Texture.Shutdown();

    m_Device.Shutdown();

    m_Context.Shutdown();
}

void Application::DrawFrame()
{
    int width;
    int height;

    glfwGetFramebufferSize(
        m_Window.GetNativeWindow(),
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


    ubo.view =
        m_Camera.GetViewMatrix();


    ubo.projection =
        m_Camera.GetProjectionMatrix();



    m_UniformBuffer.Upload(
        &ubo,
        sizeof(ubo)
    );

   // ubo.view = m_Camera.GetViewMatrix();

    //glm::vec3 cameraPos = { 3.0f, 3.0f, 3.0f };

    //glm::vec3 target = { 0.0f, 0.0f, 0.0f };

    //glm::vec3 up = { 0.0f, 1.0f, 0.0f };

    //ubo.view = glm::lookAt(
    //    cameraPos,
    //    target,
    //    up
    //);

   // ubo.projection = m_Camera.GetProjectionMatrix();

    //ubo.model =
    //    glm::mat4(1.0f);


    //ubo.view =
    //    glm::lookAt(
    //        glm::vec3(2.0f, 2.0f, 3.0f),
    //        glm::vec3(0, 0, 0),
    //        glm::vec3(0, 1, 0)
    //    );


    //ubo.projection =
    //    glm::perspective(
    //        glm::radians(45.0f),
    //        1280.0f / 720.0f,
    //        0.1f,
    //        100.0f
    //    );


    //ubo.projection[1][1] *= -1;



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

void Application::RecreateSwapchain()
{

    int width = 0;
    int height = 0;


    glfwGetFramebufferSize(
        m_Window.GetNativeWindow(),
        &width,
        &height
    );


    // 最小化窗口时等待
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(
            m_Window.GetNativeWindow(),
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
        m_Window.GetNativeWindow()
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

    m_Camera.SetAspectRatio(
        float(m_Swapchain.GetExtent().width) /
        float(m_Swapchain.GetExtent().height)
    );

}

//void Application::Run()
//{
//
//    std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXX = " << std::endl;
//
//    if (!glfwInit())
//    {
//        std::cout << "glfwInit failed\n";
//        return;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window =
//        glfwCreateWindow(1280, 720, "Daybreak", nullptr, nullptr);
//
//    if (!window)
//    {
//        std::cout << "CreateWindow failed\n";
//        return;
//    }
//
//    glfwMakeContextCurrent(window);
//
//    int ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//
//    std::cout << "GLAD load = " << ok << std::endl;
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "GLAD init failed\n";
//        return;
//    }
//
//    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
//    std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
//
//    Renderer renderer;
//    renderer.Init();
//
//
//
//    while (!glfwWindowShouldClose(window))
//    {
//        glfwPollEvents();
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        renderer.Draw();
//
//        glfwSwapBuffers(window);
//    }
//
//    glfwTerminate();
//}

