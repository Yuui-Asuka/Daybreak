#include "VulkanShader.h"

#include <fstream>
#include <stdexcept>
#include <iostream>


namespace Daybreak
{


    /**
     * @brief Initializes Vulkan shader modules.
     *
     * Loads SPIR-V shader binaries from files and creates
     * Vulkan shader modules for vertex and fragment stages.
     *
     * @param device Vulkan logical device used to create shader modules.
     * @param vertPath Path to the compiled vertex shader SPIR-V file.
     * @param fragPath Path to the compiled fragment shader SPIR-V file.
     */
    void VulkanShader::Init(
        VkDevice device,
        const std::string& vertPath,
        const std::string& fragPath)
    {

        // Store the logical device used by shader resources.
        m_Device = device;



        // Load compiled vertex shader SPIR-V binary.
        auto vertCode =
            ReadFile(vertPath);



        // Load compiled fragment shader SPIR-V binary.
        auto fragCode =
            ReadFile(fragPath);



        // Create vertex shader module.
        m_VertexShader =
            CreateShaderModule(
                vertCode
            );



        // Create fragment shader module.
        m_FragmentShader =
            CreateShaderModule(
                fragCode
            );



        std::cout
            << "Shader Modules Created!"
            << std::endl;
    }





    /**
     * @brief Reads a binary file into memory.
     *
     * Used to load SPIR-V shader files before creating
     * Vulkan shader modules.
     *
     * @param filename Path to the binary file.
     *
     * @return std::vector<char> File content stored as bytes.
     */
    std::vector<char> VulkanShader::ReadFile(
        const std::string& filename)
    {

        // Open the file in binary mode and move to the end.
        std::ifstream file(
            filename,
            std::ios::ate | std::ios::binary
        );


        if (!file.is_open())
        {
            throw std::runtime_error(
                "Failed to open shader file!"
            );
        }



        // Get file size in bytes.
        size_t fileSize =
            static_cast<size_t>(
                file.tellg()
                );



        // Allocate memory for file data.
        std::vector<char> buffer(
            fileSize
        );



        // Move file pointer back to the beginning.
        file.seekg(0);



        // Read the entire file into memory.
        file.read(
            buffer.data(),
            fileSize
        );


        file.close();


        return buffer;
    }





    /**
     * @brief Creates a Vulkan shader module from SPIR-V code.
     *
     * Converts SPIR-V binary data into a VkShaderModule
     * object that can be used by the graphics pipeline.
     *
     * @param code SPIR-V binary shader data.
     *
     * @return VkShaderModule Created Vulkan shader module.
     */
    VkShaderModule VulkanShader::CreateShaderModule(
        const std::vector<char>& code)
    {

        VkShaderModuleCreateInfo createInfo{};


        createInfo.sType =
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;



        // Size of SPIR-V binary data in bytes.
        createInfo.codeSize =
            code.size();



        // SPIR-V data must be provided as 32-bit words.
        createInfo.pCode =
            reinterpret_cast<const uint32_t*>(
                code.data()
                );



        VkShaderModule shaderModule =
            VK_NULL_HANDLE;



        if (vkCreateShaderModule(
            m_Device,
            &createInfo,
            nullptr,
            &shaderModule)
            != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create shader module!"
            );
        }


        return shaderModule;
    }





    /**
     * @brief Releases Vulkan shader modules.
     *
     * Destroys vertex and fragment shader modules
     * created during initialization.
     */
    void VulkanShader::Shutdown()
    {

        // Destroy vertex shader module.
        if (m_VertexShader != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(
                m_Device,
                m_VertexShader,
                nullptr
            );

            m_VertexShader =
                VK_NULL_HANDLE;
        }



        // Destroy fragment shader module.
        if (m_FragmentShader != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(
                m_Device,
                m_FragmentShader,
                nullptr
            );

            m_FragmentShader =
                VK_NULL_HANDLE;
        }

    }


}