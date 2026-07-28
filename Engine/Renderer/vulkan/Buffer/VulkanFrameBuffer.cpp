#include "VulkanFramebuffer.h"

#include <stdexcept>
#include <iostream>


namespace Daybreak
{

    /**
     * @brief Initializes framebuffer resources.
     *
     * A framebuffer represents a collection of image views that are
     * used as render pass attachments.
     *
     * For a typical swapchain rendering setup:
     *
     * Swapchain Image
     *        |
     *        v
     *   Image View
     *        |
     *        v
     *   Framebuffer
     *
     * Each framebuffer usually corresponds to one swapchain image.
     *
     * @param device Vulkan logical device.
     * @param renderPass Compatible render pass.
     * @param imageViews Swapchain color image views.
     * @param extent Framebuffer dimensions.
     * @param depthImageView Depth attachment image view.
     */
    void VulkanFramebuffer::Init(
        VkDevice device,
        VkRenderPass renderPass,
        const std::vector<VkImageView>& imageViews,
        VkExtent2D extent,
        VkImageView depthImageView)
    {

        m_Device = device;


        CreateFramebuffers(
            renderPass,
            imageViews,
            extent,
            depthImageView
        );
    }


    /**
     * @brief Creates framebuffer objects.
     *
     * Each framebuffer contains:
     *
     * - One swapchain color attachment.
     * - One depth attachment.
     *
     * The attachment order must match the attachment order defined
     * in the render pass.
     */
    void VulkanFramebuffer::CreateFramebuffers(
        VkRenderPass renderPass,
        const std::vector<VkImageView>& imageViews,
        VkExtent2D extent,
        VkImageView depthImageView
    )
    {

        /*
         * Create one framebuffer for each swapchain image view.
         */
        m_Framebuffers.resize(
            imageViews.size()
        );


        for (size_t i = 0;
            i < imageViews.size();
            i++)
        {

            /*
             * Attachment order must match the render pass:
             *
             * Attachment 0:
             *     Color attachment
             *
             * Attachment 1:
             *     Depth attachment
             */
            VkImageView attachments[] =
            {
                imageViews[i],
                depthImageView
            };



            VkFramebufferCreateInfo framebufferInfo{};


            framebufferInfo.sType =
                VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;



            /*
             * The framebuffer is created for this render pass.
             *
             * The render pass defines how these attachments are used
             * during rendering.
             */
            framebufferInfo.renderPass =
                renderPass;



            /*
             * Number of image views attached to this framebuffer.
             *
             * Current configuration:
             *
             * Color + Depth
             */
            framebufferInfo.attachmentCount =
                2;


            framebufferInfo.pAttachments =
                attachments;



            /*
             * Framebuffer dimensions usually match the swapchain
             * extent.
             */
            framebufferInfo.width =
                extent.width;


            framebufferInfo.height =
                extent.height;



            /*
             * Number of framebuffer layers.
             *
             * Standard window rendering uses one layer.
             */
            framebufferInfo.layers =
                1;



            if (vkCreateFramebuffer(
                m_Device,
                &framebufferInfo,
                nullptr,
                &m_Framebuffers[i])
                != VK_SUCCESS)
            {
                throw std::runtime_error(
                    "Failed to create framebuffer!"
                );
            }
        }


        std::cout
            << "Framebuffer Count: "
            << m_Framebuffers.size()
            << std::endl;
    }


    /**
     * @brief Destroys framebuffer resources.
     *
     * Framebuffers must be destroyed before their associated
     * image views and swapchain resources.
     */
    void VulkanFramebuffer::Shutdown()
    {

        for (VkFramebuffer framebuffer :
        m_Framebuffers)
        {
            if (framebuffer != VK_NULL_HANDLE)
            {
                vkDestroyFramebuffer(
                    m_Device,
                    framebuffer,
                    nullptr
                );
            }
        }

        m_Framebuffers.clear();
    }

}