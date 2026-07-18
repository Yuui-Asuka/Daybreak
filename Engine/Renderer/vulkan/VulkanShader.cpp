#include "VulkanShader.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace Daybreak
{

    void VulkanShader::Init(
        VkDevice device,
        const std::string& vertPath,
        const std::string& fragPath)
    {
        /*
            保存Logical Device。

            Shader Module属于Device资源。

            创建和销毁都需要VkDevice。
        */
        m_Device = device;


        /*
            读取Vertex Shader SPIR-V。

            文件格式：

                GLSL
                  |
                  v
                glslc编译
                  |
                  v
                .spv
                  |
                  v
                VkShaderModule

        */
        auto vertCode =
            ReadFile(vertPath);


        /*
            读取Fragment Shader SPIR-V。
        */
        auto fragCode =
            ReadFile(fragPath);



        /*
            创建Vertex Shader Module。
        */
        m_VertexShader =
            CreateShaderModule(
                vertCode
            );


        /*
            创建Fragment Shader Module。
        */
        m_FragmentShader =
            CreateShaderModule(
                fragCode
            );


        std::cout
            << "Shader Modules Created!"
            << std::endl;
    }



    std::vector<char> VulkanShader::ReadFile(
        const std::string& filename)
    {
        /*
            以二进制方式读取SPIR-V。
            ios::ate:
                打开时移动到文件末尾。
                方便获取文件大小。
            ios::binary:
                防止Windows换行转换。

        */

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


        /*
            获取文件大小。

            tellg():

                当前文件指针位置。
                因为打开时位于末尾，
                所以就是文件大小。

        */
        size_t fileSize =
            static_cast<size_t>(
                file.tellg()
                );



        /*
            创建Buffer保存SPIR-V二进制数据。
        */
        std::vector<char> buffer(
            fileSize
        );


        /*
            回到文件开头。
        */
        file.seekg(0);



        /*
            读取全部Shader数据。
        */
        file.read(
            buffer.data(),
            fileSize
        );


        file.close();


        return buffer;
    }



    VkShaderModule VulkanShader::CreateShaderModule(
        const std::vector<char>& code)
    {
        /*
            Shader Module Create Info
            Vulkan接收的是：
                SPIR-V binary
            不是：
                GLSL文本
        */

        VkShaderModuleCreateInfo createInfo{};


        createInfo.sType =
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;


        /*
            SPIR-V字节大小。
            注意：
            这里是byte数量。

        */
        createInfo.codeSize =
            code.size();



        /*
            Vulkan要求：
                const uint32_t*
            SPIR-V格式：
                32bit word数组
        */
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



    void VulkanShader::Shutdown()
    {
        /*
            销毁Vertex Shader Module。
        */
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


        /*
            销毁Fragment Shader Module。
        */
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