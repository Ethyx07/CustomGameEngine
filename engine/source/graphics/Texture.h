#pragma once
#include <GL/glew.h>

namespace eng
{
	class Texture
	{
	public:
		Texture(int wid, int heig, int channels, unsigned char* data);
		~Texture();

		GLuint GetID() const;
	private:
		int width = 0;
		int height = 0;
		int numChannels = 0;
		GLuint textureID = 0;
	};
}