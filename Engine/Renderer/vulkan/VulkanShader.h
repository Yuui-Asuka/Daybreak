#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <vector>


namespace Daybreak
{


    /**
     * @class VulkanShader
     *
     * @brief Manages Vulkan shader modules.
     *
     * Shader compilation pipeline:
     *
     * GLSL/HLSL
     *      |
     *      v
     * SPIR-V (.spv)
     *      |
     *      v
     * VkShaderModule
     *      |
     *      v
     * Graphics Pipeline
     *
     * Current supported shader stages:
     *
     * - Vertex Shader
     * - Fragment Shader
     */
    class VulkanShader
    {
    public:


        /**
         * @brief Initializes shader modules.
         *
         * Loads SPIR-V shader binaries and creates
         * Vulkan shader module objects.
         *
         * @param device Vulkan logical device used to create shader modules.
         * @param vertPath Path to the vertex shader SPIR-V file.
         * @param fragPath Path to the fragment shader SPIR-V file.
         */
        void Init(
            VkDevice device,
            const std::string& vertPath,
            const std::string& fragPath
        );



        /**
         * @brief Releases shader module resources.
         *
         * Destroys vertex and fragment shader modules.
         */
        void Shutdown();



        /**
         * @brief Retrieves the vertex shader module.
         *
         * Used by VkPipelineShaderStageCreateInfo.
         *
         * @return VkShaderModule Vertex shader module handle.
         */
        VkShaderModule GetVertexShader() const
        {
            return m_VertexShader;
        }



        /**
         * @brief Retrieves the fragment shader module.
         *
         * Used by VkPipelineShaderStageCreateInfo.
         *
         * @return VkShaderModule Fragment shader module handle.
         */
        VkShaderModule GetFragmentShader() const
        {
            return m_FragmentShader;
        }



    private:


        /**
         * @brief Creates a Vulkan shader module from SPIR-V data.
         *
         * @param code SPIR-V binary shader data.
         *
         * @return VkShaderModule Created shader module handle.
         */
        VkShaderModule CreateShaderModule(
            const std::vector<char>& code
        );



        /**
         * @brief Reads a binary shader file from disk.
         *
         * Loads compiled SPIR-V data used to create
         * Vulkan shader modules.
         *
         * @param filename Path to the shader file.
         *
         * @return std::vector<char> Shader binary data.
         */
        std::vector<char> ReadFile(
            const std::string& filename
        );



    private:


        /**
         * @brief Vulkan logical device.
         *
         * Shader modules are device-owned resources,
         * therefore creation and destruction require VkDevice.
         */
        VkDevice m_Device =
            VK_NULL_HANDLE;



        /**
         * @brief Vertex shader module.
         */
        VkShaderModule m_VertexShader =
            VK_NULL_HANDLE;



        /**
         * @brief Fragment shader module.
         */
        VkShaderModule m_FragmentShader =
            VK_NULL_HANDLE;

    };


}