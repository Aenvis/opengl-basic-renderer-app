#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: m_id(glCreateProgram())
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

	glAttachShader(m_id, vShader);
	glAttachShader(m_id, fShader);
	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::Use()
{
	glUseProgram(m_id);
}

unsigned int Shader::GetId() const
{
	return m_id;
}

void Shader::SetBool(const std::string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<int>(val));
}

void Shader::SetInt(const std::string& name, int val) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), val);
}

void Shader::SetFloat(const std::string& name, float val) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), val);
}

void Shader::SetVec3(const std::string& name, const glm::vec3 &v) const
{
	glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &v[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}

void Shader::SetMat4(const std::string& name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
