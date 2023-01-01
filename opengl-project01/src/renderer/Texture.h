#pragma once

namespace renderer
{
	class Texture
	{
	private:
		unsigned int m_textureID;
	public:
		Texture();
		Texture(const char* filename, bool isPng);
		~Texture();
		
		void Load(const char* filename, bool isPng);

		inline const unsigned int GetID() const { return m_textureID;  }
	};
}


