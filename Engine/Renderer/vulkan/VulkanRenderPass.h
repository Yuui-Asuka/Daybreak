#pragma once

#include <vulkan/vulkan.h>


namespace Daybreak
{


    /**
     * @class VulkanRenderPass
     *
     * @brief Manages Vulkan render pass resources.
     *
     * A render pass defines the structure of a rendering operation.
     *
     * It describes:
     *
     * - Render attachments
     * - Color output
     * - Depth testing resources
     * - Load and store operations
     * - Image layout transitions
     *
     * Rendering flow:
     *
     * Swapchain Image
     *        |
     *        v
     * RenderPass
     *        |
     *        v
     * Framebuffer
     *
     *
     * Lifecycle:
     *
     * Init()
     *      |
     *      v
     * Create VkRenderPass
     *
     * Shutdown()
     *      |
     *      v
     * Destroy VkRenderPass
     */
    class VulkanRenderPass
    {
    public:


        /**
         * @brief Initializes the Vulkan render pass.
         *
         * Creates a render pass using the specified
         * logical device and swapchain image format.
         *
         * @param device Vulkan logical device used to create the render pass.
         * @param imageFormat Swapchain image format used by the color attachment.
         */
        void Init(
            VkDevice device,
            VkFormat imageFormat
        );



        /**
         * @brief Releases the render pass resource.
         *
         * Destroys the VkRenderPass object.
         */
        void Shutdown();



        /**
         * @brief Retrieves the Vulkan render pass handle.
         *
         * Used when creating the graphics pipeline:
         *
         * VkGraphicsPipelineCreateInfo::renderPass
         *
         * @return VkRenderPass Render pass handle.
         */
        VkRenderPass GetRenderPass() const
        {
            return m_RenderPass;
        }



    private:


        /**
         * @brief Vulkan logical device.
         *
         * The render pass is a device-owned resource.
         *
         * Required for creation and destruction.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;



        /**
         * @brief Vulkan render pass handle.
         *
         * Defines the rendering attachments and operations.
         */
        VkRenderPass m_RenderPass =
            VK_NULL_HANDLE;

    };


}