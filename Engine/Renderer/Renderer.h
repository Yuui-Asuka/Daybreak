#pragma once


#include <glad/glad.h>
#include <iostream>

class Renderer
{
public:
    void Init();
    void Draw();

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint ShaderProgram = 0;

    GLint offsetLoc = -1;   

    GLuint CompileShader(GLenum type, const char* src);
    GLuint CreateProgram(const char* vs, const char* fs);
};