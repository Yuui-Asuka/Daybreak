#pragma once

#include <vulkan/vulkan.h>

#include <vector>


namespace Daybreak
{

    /**
     * @brief Manages Vulkan framebuffer resources.
     *
     * A framebuffer represents a collection of image views that are
     * used as attachments during render pass execution.
     *
     * Relationship:
     *
     * Swapchain Image
     *        |
     *        v
     * Image View
     *        |
     *        v
     * Framebuffer
     *        |
     *        v
     * Render Pass
     *
     *
     * Render Pass defines:
     *
     * - How attachments are used.
     * - Rendering operations.
     * - Attachment transitions.
     *
     *
     * Framebuffer defines:
     *
     * - Which image views are used as attachments.
     *
     *
     * Typical swapchain setup:
     *
     * Image[0] -> Framebuffer[0]
     *
     * Image[1] -> Framebuffer[1]
     *
     * Image[2] -> Framebuffer[2]
     */
    class VulkanFramebuffer
    {
    public:

        /**
         * @brief Creates framebuffer resources.
         *
         * Each framebuffer is created from a swapchain image view
         * and a depth image view.
         *
         * The attachment order must match the attachment order
         * defined in the render pass.
         *
         * @param device Vulkan logical device.
         * @param renderPass Compatible render pass.
         * @param imageViews Swapchain color image views.
         * @param extent Framebuffer resolution.
         * @param depthImageView Depth attachment image view.
         */
        void Init(
            VkDevice device,
            VkRenderPass renderPass,
            const std::vector<VkImageView>& imageViews,
            VkExtent2D extent,
            VkImageView depthImageView
        );


        /**
         * @brief Destroys all framebuffer resources.
         */
        void Shutdown();


        /**
         * @brief Returns all created framebuffers.
         *
         * Command buffers use these framebuffers when beginning
         * a render pass.
         *
         * @return Collection of framebuffer handles.
         */
        const std::vector<VkFramebuffer>& GetFramebuffers() const
        {
            return m_Framebuffers;
        }



    private:

        /**
         * @brief Creates framebuffer objects.
         *
         * Creates one framebuffer for each swapchain image view.
         *
         * Example:
         *
         * Swapchain image count = 3
         *
         * Creates:
         *
         * Framebuffer[0]
         * Framebuffer[1]
         * Framebuffer[2]
         *
         * @param renderPass Compatible render pass.
         * @param imageViews Swapchain image views.
         * @param extent Framebuffer dimensions.
         * @param depthImageView Depth attachment image view.
         */
        void CreateFramebuffers(
            VkRenderPass renderPass,
            const std::vector<VkImageView>& imageViews,
            VkExtent2D extent,
            VkImageView depthImageView
        );



    private:

        /**
         * @brief Vulkan logical device.
         *
         * Used for framebuffer creation and destruction.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /**
         * @brief Collection of framebuffer handles.
         *
         * Usually contains one framebuffer per swapchain image.
         */
        std::vector<VkFramebuffer> m_Framebuffers;

    };

}