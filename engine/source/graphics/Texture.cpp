#include "graphics/Texture.h"

namespace eng
{
	Texture::Texture(int wid, int heig, int channels, unsigned char* data)
		:width(wid), height(heig), numChannels(channels)
	{
		glGenTextures(1, &textureID); //Generates this texture
		glBindTexture(GL_TEXTURE_2D, textureID); //Binds/Activates the texture with that ID

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Repeats the texture on both s and t axis (u and v)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Controls texture as its shrunk (moving away)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Controls how its displayed when zoomed/closer
	}

	Texture::~Texture()
	{
		if (textureID > 0)
		{
			glDeleteTextures(1, &textureID); //If id == 0 then texture has not been generated. Otherwise it is destroyed to free space/memory
		}
	}

	GLuint Texture::GetID() const
	{
		return textureID;
	}
}