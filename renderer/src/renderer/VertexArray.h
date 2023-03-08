#pragma once

namespace renderer
{
	class VertexArray
	{
	private:
		unsigned int m_arrayID;

	public:
		VertexArray();
		~VertexArray();

		void VertexAttribPtr(unsigned int id, unsigned int count, unsigned int stride, void* offset);

		void Bind();
		void Unbind();
	};
}


