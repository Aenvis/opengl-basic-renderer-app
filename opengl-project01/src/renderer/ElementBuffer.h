#pragma once
#include <glad/glad.h> 

namespace renderer::buffers
{
	class ElementBuffer
	{
	private:
		unsigned int m_bufferID;
		unsigned int m_Count;

	public:
		ElementBuffer(const void* data, unsigned int count);
		~ElementBuffer();

		void Bind() const;
		void Unbind() const;

		inline const unsigned int GetCount() const { return m_Count; }
	};
}
