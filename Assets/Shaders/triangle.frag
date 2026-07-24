#version 450


// Vertex Shader 输出

layout(location = 0) in vec3 fragColor;

layout(location = 1) in vec2 fragTexCoord;



// 输出到 Framebuffer

layout(location = 0) out vec4 outColor;



// Texture Sampler

// 对应 DescriptorSetLayout:
//
// binding = 1
// VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
//

layout(binding = 1) uniform sampler2D texSampler;



void main()
{

    vec4 textureColor =
        texture(
            texSampler,
            fragTexCoord
        );


    /*
        纹理颜色 × 顶点颜色

        当前 Vertex 颜色是白色：
        (1,1,1)

        所以实际效果就是显示原纹理。

        以后改 Vertex 颜色可以做染色效果。
    */

    outColor =
        textureColor *
        vec4(fragColor, 1.0);

}