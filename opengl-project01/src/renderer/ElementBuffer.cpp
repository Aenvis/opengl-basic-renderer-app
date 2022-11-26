#include "ElementBuffer.h"

namespace renderer::buffers
{
	ElementBuffer::ElementBuffer(const void* data, unsigned int count)
		: m_Count(count)
	{
		glGenBuffers(1, &m_bufferID);
		this->Bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	}

	ElementBuffer::~ElementBuffer()
	{
		this->Unbind();
		glDeleteBuffers(1, &m_bufferID);
	}

	void ElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
	}
	void ElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}