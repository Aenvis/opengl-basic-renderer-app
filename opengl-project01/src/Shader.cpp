#include <glad/glad.h>
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: ID(glCreateProgram())
{
	std::string vertexSourceCode;
	std::string fragmentSourceCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream; // v stands for vertex, f for fragment
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();
		vertexSourceCode = vShaderStream.str();
		fragmentSourceCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
	}

	const char* vShaderCode = vertexSourceCode.c_str();
	const char* fShaderCode = fragmentSourceCode.c_str();

	unsigned int vShader, fShader;
	int success;
	char infoLog[512];

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vShaderCode, NULL);
	glCompileShader(vShader);

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderCode, NULL);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(val));
}

void Shader::SetInt(const std::string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::SetFloat(const std::string& name, bool val) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}
