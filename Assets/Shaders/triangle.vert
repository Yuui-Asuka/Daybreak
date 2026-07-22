#version 450


// Vertex Buffer
// 对应 Vertex.h

layout(location = 0) in vec3 inPosition;

layout(location = 1) in vec3 inColor;


// 输出给 Fragment Shader

layout(location = 0) out vec3 fragColor;



// Uniform Buffer

layout(binding = 0) uniform UniformBufferObject
{

    mat4 model;

    mat4 view;

    mat4 projection;

} ubo;



void main()
{

    gl_Position =
        ubo.projection *
        ubo.view *
        ubo.model *
        vec4(inPosition, 1.0);



    fragColor = inColor;

}