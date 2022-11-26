#pragma once
#include <glad/glad.h> 

namespace renderer
{
	class VertexBuffer
	{
	private:
		unsigned int m_bufferID;

	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	};
}
