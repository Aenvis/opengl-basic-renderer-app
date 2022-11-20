#pragma once
#include <string>
class Shader
{
private:
	unsigned int m_id;

public:

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void Use();
	unsigned int GetId() const;
	void SetBool(const std::string& name, bool val) const;
	void SetInt(const std::string& name, int val) const;
	void SetFloat(const std::string& name, float val) const;
};

