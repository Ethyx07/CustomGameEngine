#pragma once
#include <GL/glew.h>
#include <memory>
#include <string>

namespace eng
{
	class Texture
	{
	public:
		Texture(int wid, int heig, int channels, unsigned char* data);
		~Texture();
		GLuint GetID() const;
		void Init(int wid, int heig, int channels, unsigned char* data);

		static std::shared_ptr<Texture> Load(const std::string& path);
	private:
		int width = 0;
		int height = 0;
		int numChannels = 0;
		GLuint textureID = 0;
	};
}