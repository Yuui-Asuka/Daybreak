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


    /**
     * @class VulkanRenderer
     *
     * @brief Manages the Vulkan rendering pipeline.
     *
     * VulkanRenderer is responsible for coordinating all
     * rendering-related Vulkan resources.
     *
     * Main responsibilities:
     *
     * - Initialize Vulkan rendering resources
     * - Manage swapchain rendering
     * - Submit command buffers
     * - Update per-frame data
     * - Handle window resizing
     * - Release rendering resources
     *
     * Rendering flow:
     *
     * Application
     *      |
     *      v
     * VulkanRenderer
     *      |
     *      +-- VulkanContext
     *      |
     *      +-- VulkanDevice
     *      |
     *      +-- Swapchain
     *      |
     *      +-- RenderPass
     *      |
     *      +-- Pipeline
     *      |
     *      +-- CommandBuffer
     *      |
     *      +-- Synchronization
     */
    class VulkanRenderer
    {

    public:


        /**
         * @brief Initializes Vulkan renderer resources.
         *
         * Creates:
         *
         * - Vulkan instance
         * - GPU device
         * - Swapchain
         * - Render pass
         * - Shader modules
         * - Graphics pipeline
         * - Buffers
         * - Descriptor resources
         * - Command buffers
         * - Synchronization objects
         *
         * @param window GLFW window used as rendering target.
         */
        void Init(
            GLFWwindow* window
        );



        /**
         * @brief Releases all Vulkan renderer resources.
         *
         * Destroys rendering resources in reverse dependency order.
         */
        void Shutdown();



        /**
         * @brief Renders one frame.
         *
         * Executes:
         *
         * - Acquire swapchain image
         * - Update GPU resources
         * - Submit command buffer
         * - Present rendered image
         */
        void DrawFrame();



        /**
         * @brief Sets the active rendering camera.
         *
         * The camera provides view and projection matrices
         * for shader transformation.
         *
         * @param camera Camera used for rendering.
         */
        void SetCamera(
            Camera* camera
        );


    private:


        /**
         * @brief Recreates swapchain-dependent resources.
         *
         * Called when the swapchain becomes invalid,
         * such as after window resizing.
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
        void RecreateSwapchain();



    private:


        /**
         * @brief GLFW rendering window.
         *
         * Used for creating Vulkan surface
         * and querying framebuffer size.
         */
        GLFWwindow* m_Window = nullptr;



        /**
         * @brief Vulkan global context.
         *
         * Manages Vulkan instance and window surface.
         */
        VulkanContext m_Context;



        /**
         * @brief Vulkan GPU device manager.
         *
         * Manages physical device,
         * logical device and queues.
         */
        VulkanDevice m_Device;



        /**
         * @brief Command pool manager.
         *
         * Allocates Vulkan command buffers.
         */
        VulkanCommandPool m_CommandPool;



        /**
         * @brief Swapchain manager.
         *
         * Handles images presented to the window.
         */
        VulkanSwapchain m_Swapchain;



        /**
         * @brief Render pass manager.
         *
         * Defines framebuffer attachments
         * and rendering operations.
         */
        VulkanRenderPass m_RenderPass;



        /**
         * @brief Shader module manager.
         *
         * Loads and manages Vulkan shader modules.
         */
        VulkanShader m_Shader;



        /**
         * @brief Graphics pipeline manager.
         *
         * Defines GPU rendering states.
         */
        VulkanPipeline m_Pipeline;



        /**
         * @brief Texture resource manager.
         *
         * Handles GPU texture creation and access.
         */
        VulkanTexture m_Texture;



        /**
         * @brief Descriptor set layout manager.
         *
         * Defines shader resource bindings.
         */
        VulkanDescriptorSetLayout m_DescriptorSetLayout;



        /**
         * @brief Descriptor pool manager.
         *
         * Allocates descriptor sets.
         */
        VulkanDescriptorPool m_DescriptorPool;



        /**
         * @brief Descriptor set manager.
         *
         * Stores resources bound to shaders.
         */
        VulkanDescriptorSet m_DescriptorSet;



        /**
         * @brief Vertex buffer.
         *
         * Stores mesh vertex data on GPU.
         */
        VulkanBuffer m_VertexBuffer;



        /**
         * @brief Index buffer.
         *
         * Stores mesh index data on GPU.
         */
        VulkanIndexBuffer m_VulkanIndexBuffer;



        /**
         * @brief Uniform buffer.
         *
         * Stores per-frame shader parameters,
         * such as transformation matrices.
         */
        VulkanUniformBuffer m_UniformBuffer;



        /**
         * @brief Depth buffer.
         *
         * Stores depth information for depth testing.
         */
        VulkanDepthBuffer m_DepthBuffer;



        /**
         * @brief Framebuffer manager.
         *
         * Creates framebuffer objects
         * for swapchain images.
         */
        VulkanFramebuffer m_Framebuffer;



        /**
         * @brief Command buffer manager.
         *
         * Records GPU rendering commands.
         */
        VulkanCommandBuffer m_CommandBuffer;



        /**
         * @brief Synchronization manager.
         *
         * Manages Vulkan fences and semaphores.
         */
        VulkanSync m_Sync;



        /**
         * @brief Active rendering camera.
         *
         * Provides view and projection matrices.
         */
        Camera* m_Camera = nullptr;

    };


}