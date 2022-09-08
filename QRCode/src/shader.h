#pragma once

#include <string>

class Shader
{
public:
    Shader();
    void CompileShaders(const char* vertex_path, const char* fragment_path);
    ~Shader();

    unsigned int GetShaderProgram()
    {
        return m_shader_program;
    }

    void Use();
    void Delete();

private:
    unsigned int m_shader_program = 0;

    const std::string ParseShader(const char* shader_path);
};