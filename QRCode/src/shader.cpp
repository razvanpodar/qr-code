#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "GL/glew.h"

Shader::Shader()
{
	// Empty
}

void Shader::CompileShaders(const char* vertex_path, const char* fragment_path)
{
	int success;
	char info_log[512];

	const std::string vertex_source = ParseShader(vertex_path);
	const std::string fragment_source = ParseShader(fragment_path);

	const char* vs_source = vertex_source.c_str();
	const char* fs_source = fragment_source.c_str();

	std::cout << vs_source << std::endl;
	std::cout << fs_source << std::endl;

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vs_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	};

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fs_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	};

	m_shader_program = glCreateProgram();
	glAttachShader(m_shader_program, vertex_shader);
	glAttachShader(m_shader_program, fragment_shader);
	glLinkProgram(m_shader_program);

	glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shader_program, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
	glDeleteProgram(m_shader_program);
}

void Shader::Use()
{
	glUseProgram(m_shader_program);
}

void Shader::Delete()
{
	glDeleteProgram(m_shader_program);
}

const std::string Shader::ParseShader(const char* shader_path)
{
	std::string shader_source;
	std::ifstream shader_file;

	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shader_file.open(shader_path);

		std::stringstream shader_stream;
		shader_stream << shader_file.rdbuf();
		shader_file.close();
		shader_source = shader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return shader_source;
}