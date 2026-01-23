#include "graphics/Texture.h"
#include "Engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace eng
{
	Texture::Texture(int wid, int heig, int channels, unsigned char* data)
		:width(wid), height(heig), numChannels(channels)
	{
		Init(wid, heig, channels, data);
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

	void Texture::Init(int wid, int heig, int channels, unsigned char* data)
	{
		glGenTextures(1, &textureID); //Generates this texture
		glBindTexture(GL_TEXTURE_2D, textureID); //Binds/Activates the texture with that ID

		GLint internalFormat = GL_RGB;
		GLenum format = GL_RGB;

		if (numChannels == 4) //Allows for textures with 4 channels to be loaded correctly
		{
			internalFormat = GL_RGBA;
			format = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Repeats the texture on both s and t axis (u and v)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Controls texture as its shrunk (moving away)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Controls how its displayed when zoomed/closer
	}

	int Texture::GetWidth() const
	{
		return width;
	}

	int Texture::GetHeight() const
	{
		return height;
	}

	std::shared_ptr<Texture> Texture::Load(const std::string& path)
	{
		int width, height, numChannels;
		auto& fs = eng::Engine::GetInstance().GetFileSystem(); //Gets the file system where assets are stored
		auto fullPath = fs.GetAssetsFolder() / path; //Specifically gets the path for the brick png

		if (!std::filesystem::exists(fullPath))
		{
			return nullptr;
		}

		std::shared_ptr<Texture> result;
		unsigned char* data = stbi_load(fullPath.string().c_str(), &width, &height, &numChannels, 0); //Loads texture data
		
		if (data)
		{
			result = std::make_shared<Texture>(width, height, numChannels, data);
			stbi_image_free(data);
			
		}

		return result;
	}

	std::shared_ptr<Texture> TextureManager::GetOrLoadTexture(const std::string& path) //Checks if texture manager has texture loaded already (path is key)
	{
		auto it = textures.find(path);
		if (it != textures.end()) //If already in map, return it instead of loading it again
		{
			return it->second;
		}

		auto texture = Texture::Load(path); //If its not in the manager load it instead
		textures[path] = texture;
		return texture;
	}
}