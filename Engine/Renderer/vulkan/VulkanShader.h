#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace Daybreak
{

    /*
        VulkanShader

        管理 Vulkan Shader Module。

        Shader流程：

            GLSL/HLSL
                |
                v

            SPIR-V (.spv)
                |
                v

            VkShaderModule
                |
                v
            Graphics Pipeline

        当前：

            Vertex Shader
            Fragment Shader


    */
    class VulkanShader
    {
    public:

        /*
            创建Shader Module。
            device:
                Vulkan Logical Device
            vertPath:
                Vertex Shader SPIR-V文件路径
            fragPath:
                Fragment Shader SPIR-V文件路径

        */
        void Init(
            VkDevice device,
            const std::string& vertPath,
            const std::string& fragPath
        );


        /*
            销毁Shader Module资源。
        */
        void Shutdown();


        /*
            获取Vertex Shader Module。
            用于：
                VkPipelineShaderStageCreateInfo

        */
        VkShaderModule GetVertexShader() const
        {
            return m_VertexShader;
        }


        /*
            获取Fragment Shader Module。
            用于：
                VkPipelineShaderStageCreateInfo

        */
        VkShaderModule GetFragmentShader() const
        {
            return m_FragmentShader;
        }


    private:

        /*
            根据SPIR-V二进制创建VkShaderModule。
            输入：
                .spv文件内容
            输出：
                VkShaderModule

        */
        VkShaderModule CreateShaderModule(
            const std::vector<char>& code
        );


        /*
            从磁盘读取SPIR-V文件。
            Vulkan Shader不是直接读取文本。
            需要读取编译后的二进制。

        */
        std::vector<char> ReadFile(
            const std::string& filename
        );


    private:

        /*
            Vulkan Logical Device。
            Shader Module属于Device资源。

        */
        VkDevice m_Device =
            VK_NULL_HANDLE;


        /*
            Vertex Shader Module
        */
        VkShaderModule m_VertexShader =
            VK_NULL_HANDLE;


        /*
            Fragment Shader Module
        */
        VkShaderModule m_FragmentShader =
            VK_NULL_HANDLE;

    };

}