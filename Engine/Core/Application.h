#pragma once
#include <Window/Window.h>
#include <Renderer/vulkan/VulkanContext.h>
#include <Renderer/vulkan/VulkanDevice.h>
#include <Renderer/vulkan/VulkanSwapchain.h>
#include <Renderer/vulkan/VulkanRenderPass.h>
#include <Renderer/vulkan/VulkanShader.h>
#include <Renderer/vulkan/VulkanPipeline.h>
#include <Renderer/vulkan/VulkanFrameBuffer.h>
#include <Renderer/vulkan/VulkanCommandPool.h>
#include <Renderer/vulkan/VulkanCommandBuffer.h>
#include <Renderer/vulkan/VulkanSync.h>
#include <Renderer/vulkan/VulkanBuffer.h>
#include <Renderer/Mesh/Mesh.h>
#include <Renderer/Buffer/VulkanIndexBuffer.h>
#include "Renderer/Vulkan/VulkanUniformBuffer.h"
#include "Renderer/Vulkan/Descriptor/VulkanDescriptorSetLayout.h"
#include "Renderer/Vulkan/Descriptor/VulkanDescriptorPool.h"
#include "Renderer/Vulkan/Descriptor/VulkanDescriptorSet.h"


class Application
{
public:
    
    Application();
    void Run();
    void RunVulkan();
    void DrawFrame();

private:
    Daybreak::Window m_Window;
    Daybreak::VulkanContext m_Context;
    Daybreak::VulkanDevice m_Device;
    Daybreak::VulkanSwapchain m_Swapchain;
    Daybreak::VulkanRenderPass m_RenderPass;
    Daybreak::VulkanShader m_Shader;
    Daybreak::VulkanPipeline m_Pipeline;
    Daybreak::VulkanFramebuffer m_Framebuffer;
    Daybreak::VulkanCommandPool m_CommandPool;
    Daybreak::VulkanCommandBuffer m_CommandBuffer;
    Daybreak::VulkanSync m_Sync;
    Daybreak::VulkanBuffer m_VertexBuffer;
    Daybreak::Mesh m_TestMesh;
    Daybreak::VulkanIndexBuffer m_VulkanIndexBuffer;
    Daybreak::VulkanUniformBuffer m_UniformBuffer;
    Daybreak::VulkanDescriptorSetLayout m_DescriptorSetLayout;
    Daybreak::VulkanDescriptorPool m_DescriptorPool;
    Daybreak::VulkanDescriptorSet m_DescriptorSet;

private:

    void RecreateSwapchain();
};