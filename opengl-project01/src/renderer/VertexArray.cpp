#include "VertexArray.h"
#include <glad/glad.h> 

namespace renderer
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_arrayID);
		glBindVertexArray(m_arrayID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_arrayID);
	}
	void VertexArray::VertexAttribPtr(unsigned int id, unsigned int size, unsigned int stride, void* offset)
	{
		glEnableVertexAttribArray(id);
		glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, stride, offset);
	}
	void VertexArray::Bind()
	{
		glBindVertexArray(m_arrayID);
	}
	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
}

