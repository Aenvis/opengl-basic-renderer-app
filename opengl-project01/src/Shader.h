#pragma once
#include <string>
class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void Use();
	void SetBool(const std::string& name, bool val) const;
	void SetInt(const std::string& name, int val) const;
	void SetFloat(const std::string& name, float val) const;
};

