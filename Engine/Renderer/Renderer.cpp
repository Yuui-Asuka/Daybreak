#include "Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


void Renderer::Draw()
{
    glUseProgram(ShaderProgram);

    float time = (float)glfwGetTime();
    float offset = sin(time) * 0.5f;

    glUniform1f(offsetLoc, offset);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform float uOffset;

void main()
{
    gl_Position = vec4(aPos.x + uOffset, aPos.y, aPos.z, 1.0);
}
)";

static const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

GLuint Renderer::CompileShader(GLenum type, const char* src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cout << "Shader compile error:\n" << log << std::endl;
        return 0;
    }

    return shader;
}

GLuint Renderer::CreateProgram(const char* vs, const char* fs)
{
    GLuint v = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint f = CompileShader(GL_FRAGMENT_SHADER, fs);

    if (v == 0 || f == 0)
        return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cout << "Program link error:\n" << log << std::endl;
        return 0;
    }

    glDeleteShader(v);
    glDeleteShader(f);

    return program;
}

void Renderer::Init()
{
    float vertices[] = {
         -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.0f,  0.5f, 0.0f
    };

    // ===== Shader =====
    ShaderProgram = CreateProgram(vertexShaderSrc, fragmentShaderSrc);
    GLint offsetLoc = glGetUniformLocation(ShaderProgram, "uOffset");


    std::cout << "offsetLoc = " << offsetLoc << std::endl;

    if (ShaderProgram == 0)
    {
        std::cout << "Shader program creation failed!" << std::endl;
        return;
    }

    // ===== VAO =====
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // ===== VBO =====
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ===== attribute =====
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind（推荐）
    glBindVertexArray(0);



    std::cout << "Renderer Init OK" << std::endl;
}