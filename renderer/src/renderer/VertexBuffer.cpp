#include "VertexBuffer.h"

namespace renderer
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_bufferID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_VERTEX_ARRAY, m_bufferID);
	}
	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_VERTEX_ARRAY, 0);
	}
}