#include "Texture.h"
#include <iostream>
#include <glad/glad.h>
#include "../stb_image.h"

namespace renderer
{
	Texture::Texture()
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
	Texture::~Texture()
	{
	}
	void Texture::Load(const char* filename, bool isPng)
	{
		int width=0, height=0, nrChannels=0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, isPng ? 4 : 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, isPng ? GL_RGBA : GL_RGB, width, height, 0, isPng ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "FAILED TO LOAD TEXTURE\n";
		}
		stbi_image_free((void*)data);
	}
}