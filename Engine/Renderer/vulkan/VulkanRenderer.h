#pragma once


#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "Buffer/VulkanFramebuffer.h"
#include "Buffer/VulkanCommandBuffer.h"
#include "VulkanSync.h"
#include "../Texture/VulkanTexture.h"
#include "Buffer/VulkanBuffer.h"
#include "Buffer/VulkanIndexBuffer.h"
#include "Buffer/VulkanUniformBuffer.h"
#include "Descriptor/VulkanDescriptorSet.h"
#include "Descriptor/VulkanDescriptorPool.h"
#include "Descriptor/VulkanDescriptorSetLayout.h"
#include "Buffer/VulkanDepthBuffer.h"
#include "../../Scene/Camera/Camera.h"


namespace Daybreak
{


    class VulkanRenderer
    {

    public:


        void Init(
            GLFWwindow* window
        );


        void Shutdown();

        void DrawFrame();

        void SetCamera(
            Camera* camera
        );


    private:


        

        void RecreateSwapchain();



    private:


        GLFWwindow* m_Window = nullptr;



        VulkanContext m_Context;


        VulkanDevice m_Device;


        VulkanCommandPool m_CommandPool;



        VulkanSwapchain m_Swapchain;


        VulkanRenderPass m_RenderPass;


        VulkanShader m_Shader;


        VulkanPipeline m_Pipeline;



        VulkanTexture m_Texture;



        VulkanDescriptorSetLayout m_DescriptorSetLayout;


        VulkanDescriptorPool m_DescriptorPool;


        VulkanDescriptorSet m_DescriptorSet;



        VulkanBuffer m_VertexBuffer;


        VulkanIndexBuffer m_VulkanIndexBuffer;


        VulkanUniformBuffer m_UniformBuffer;



        VulkanDepthBuffer m_DepthBuffer;


        VulkanFramebuffer m_Framebuffer;


        VulkanCommandBuffer m_CommandBuffer;



        VulkanSync m_Sync;

        Camera* m_Camera = nullptr;

    };


}