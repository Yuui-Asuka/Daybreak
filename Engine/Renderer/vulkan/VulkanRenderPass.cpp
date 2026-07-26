#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

#include "VulkanRenderPass.h"


namespace Daybreak
{

    /**
     * @brief Initializes the Vulkan render pass.
     *
     * Creates the render pass configuration used during rendering.
     *
     * The render pass defines:
     *
     * - Color attachment
     * - Depth attachment
     * - Subpass operations
     * - Pipeline synchronization dependencies
     *
     * @param device Vulkan logical device used to create the render pass.
     * @param imageFormat Swapchain image format used by the color attachment.
     */
    void VulkanRenderPass::Init(
        VkDevice device,
        VkFormat imageFormat
    )
    {

        // Store the logical device used for resource management.
        m_Device = device;



        /**
         * @brief Defines the color attachment.
         *
         * The color attachment represents the swapchain image
         * used as the rendering target.
         */
        VkAttachmentDescription colorAttachment{};


        // Use the same format as the swapchain image.
        colorAttachment.format =
            imageFormat;


        // Disable multisampling.
        colorAttachment.samples =
            VK_SAMPLE_COUNT_1_BIT;



        // Clear the image before rendering begins.
        colorAttachment.loadOp =
            VK_ATTACHMENT_LOAD_OP_CLEAR;


        // Preserve the rendered image for presentation.
        colorAttachment.storeOp =
            VK_ATTACHMENT_STORE_OP_STORE;



        // No stencil attachment is used.
        colorAttachment.stencilLoadOp =
            VK_ATTACHMENT_LOAD_OP_DONT_CARE;

        colorAttachment.stencilStoreOp =
            VK_ATTACHMENT_STORE_OP_DONT_CARE;



        // Initial image state before rendering.
        colorAttachment.initialLayout =
            VK_IMAGE_LAYOUT_UNDEFINED;


        // Final image state for presentation.
        colorAttachment.finalLayout =
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;



        /**
         * @brief References the color attachment inside the subpass.
         */
        VkAttachmentReference colorAttachmentRef{};


        // Reference the first attachment.
        colorAttachmentRef.attachment = 0;


        // Image layout required for color output.
        colorAttachmentRef.layout =
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;




        /**
         * @brief Defines the depth attachment.
         *
         * Used for depth testing during rasterization.
         */
        VkAttachmentDescription depthAttachment{};


        // Use 32-bit floating point depth format.
        depthAttachment.format =
            VK_FORMAT_D32_SFLOAT;


        // Disable multisampling.
        depthAttachment.samples =
            VK_SAMPLE_COUNT_1_BIT;


        // Clear depth buffer before rendering.
        depthAttachment.loadOp =
            VK_ATTACHMENT_LOAD_OP_CLEAR;


        // Depth data is not needed after rendering.
        depthAttachment.storeOp =
            VK_ATTACHMENT_STORE_OP_DONT_CARE;


        // No stencil operations.
        depthAttachment.stencilLoadOp =
            VK_ATTACHMENT_LOAD_OP_DONT_CARE;

        depthAttachment.stencilStoreOp =
            VK_ATTACHMENT_STORE_OP_DONT_CARE;



        // Initial depth image state.
        depthAttachment.initialLayout =
            VK_IMAGE_LAYOUT_UNDEFINED;


        // Final depth image state.
        depthAttachment.finalLayout =
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;



        /**
         * @brief References the depth attachment inside the subpass.
         */
        VkAttachmentReference depthAttachmentRef{};


        // Reference the second attachment.
        depthAttachmentRef.attachment =
            1;


        // Layout required for depth testing.
        depthAttachmentRef.layout =
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;




        /**
         * @brief Defines a graphics rendering subpass.
         *
         * A subpass describes one rendering operation.
         *
         * Current pipeline:
         *
         * Vertex Shader
         *      |
         * Fragment Shader
         *      |
         * Color Attachment
         */
        VkSubpassDescription subpass{};


        // This subpass belongs to the graphics pipeline.
        subpass.pipelineBindPoint =
            VK_PIPELINE_BIND_POINT_GRAPHICS;


        // Use one color attachment.
        subpass.colorAttachmentCount =
            1;


        subpass.pColorAttachments =
            &colorAttachmentRef;


        // Attach depth buffer.
        subpass.pDepthStencilAttachment =
            &depthAttachmentRef;




        /**
         * @brief Defines synchronization between pipeline stages.
         *
         * Ensures that rendering operations access attachments
         * at the correct time.
         */
        VkSubpassDependency dependency{};


        // Dependency from operations outside this render pass.
        dependency.srcSubpass =
            VK_SUBPASS_EXTERNAL;


        // Dependency to the first subpass.
        dependency.dstSubpass =
            0;



        // Wait for color attachment output stage.
        dependency.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;


        dependency.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;



        // No previous access requirement.
        dependency.srcAccessMask =
            0;


        // Allow color attachment writes.
        dependency.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;




        // Combine all attachments.
        VkAttachmentDescription attachments[] =
        {
            colorAttachment,
            depthAttachment
        };



        /**
         * @brief Creates render pass configuration.
         *
         * Combines:
         *
         * - Attachments
         * - Subpasses
         * - Dependencies
         */
        VkRenderPassCreateInfo renderPassInfo{};


        renderPassInfo.sType =
            VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;


        // Specify attachment list.
        renderPassInfo.attachmentCount =
            2;


        renderPassInfo.pAttachments =
            attachments;



        // Specify subpass list.
        renderPassInfo.subpassCount =
            1;


        renderPassInfo.pSubpasses =
            &subpass;



        // Specify dependency list.
        renderPassInfo.dependencyCount =
            1;


        renderPassInfo.pDependencies =
            &dependency;



        // Create Vulkan render pass.
        if (vkCreateRenderPass(
            m_Device,
            &renderPassInfo,
            nullptr,
            &m_RenderPass)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create RenderPass!"
            );
        }


        std::cout
            << "RenderPass Created!"
            << std::endl;

    }




    /**
     * @brief Destroys the Vulkan render pass.
     *
     * Releases the render pass resource owned by this object.
     */
    void VulkanRenderPass::Shutdown()
    {

        // Destroy render pass if it exists.
        if (m_RenderPass != VK_NULL_HANDLE)
        {

            vkDestroyRenderPass(
                m_Device,
                m_RenderPass,
                nullptr
            );


            // Reset handle after destruction.
            m_RenderPass =
                VK_NULL_HANDLE;
        }

    }


}