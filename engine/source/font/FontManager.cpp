#include "font/FontManager.h"
#include "Engine.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

namespace eng
{
	void FontManager::Init()
	{
		FT_Error error = FT_Init_FreeType(&fontLibrary);
		if (error != FT_Err_Ok)
		{
			//Init failed
			fontLibrary = nullptr;
			return;
		}
	}

	FontManager::~FontManager()
	{
		if (fontLibrary)
		{
			FT_Done_FreeType(fontLibrary);
			fontLibrary = nullptr;
		}
	}

	std::shared_ptr<Font> FontManager::GetFont(const std::string& path, int size)
	{
		//Checks cache for font
		auto fontFamIterator = fonts.find(path);
		if (fontFamIterator != fonts.end())
		{
			auto fontIterator = fontFamIterator->second.find(size);
			if (fontIterator != fontFamIterator->second.end())
			{
				return fontIterator->second;
			}
		}

		//Load font file into memory if it isnt in cache
		auto buffer = Engine::GetInstance().GetFileSystem().LoadAssetFile(path);
		if (buffer.empty())
		{
			std::cout << "[ERROR: Font Path Not Valid. Please Ensure Font is Stored in assets/fonts folder and the correct path is used in function]" << std::endl;
			return nullptr; //Means path does not lead to a valid file
		}

		//Create freetype face
		FT_Face face;
		FT_Error result = FT_New_Memory_Face(
			fontLibrary, reinterpret_cast<FT_Byte*>(buffer.data()), buffer.size(),
			0, &face);
		if (result != FT_Err_Ok) //Makes sure freetype face is created properly
		{
			return nullptr;
		}

		//Sets the pixel size
		FT_Set_Pixel_Sizes(face, 0, size);

		const int lineHeight = (face->size->metrics.height >> 6);
		int maxDimension = static_cast<int>(std::sqrtf(128.0f) * (lineHeight + 1));
		int textureWidth = 1;
		while (textureWidth < maxDimension)
		{
			textureWidth <<= 1;
		}

		int textureHeight = textureWidth;

		//Creates RGBA Buffer and fills with zeros (transparent val)
		const size_t stride = textureWidth * 4;
		const size_t totalBytes = static_cast<size_t>(textureWidth) * textureHeight * 4;
		auto* atlas = new unsigned char[totalBytes];
		std::memset(atlas, 0, totalBytes);

		//Rasterize ASCII 0-127 into the atlas
		int penX = 0;
		int penY = 0;

		auto font = std::make_shared<Font>();

		for (int c = 0; c < 128; c++)
		{
			//Load and render glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER) != FT_Err_Ok)
			{
				//Failed glyph, skipped but keep metrics same
				font->descriptions[c] = { 0,0,0,0, 0,0, 0 };
				continue;
			}

			FT_Bitmap& bmp = face->glyph->bitmap;

			//New line if overflow occurs
			if (penX + static_cast<int>(bmp.width) >= textureWidth)
			{
				penX = 0;
				penY += lineHeight + 1;
			}

			//Copy glyph bitmap (greyscale) to RGBA atlas
			for (uint32_t row = 0; row < bmp.rows; row++)
			{
				for (uint32_t col = 0; col < bmp.width; col++)
				{
					int x = penX + static_cast<int>(col);
					int y = penY + static_cast<int>(row);
					if (x < 0 || x >= textureWidth || y < 0 || y >= textureHeight) //X must be within 0-width and y 0-height
					{
						continue;
					}

					const unsigned char value = bmp.buffer[row * bmp.pitch + col];
					const size_t index = static_cast<size_t>(y) * stride + x * 4;

					//Write greyscale into all RGBA channels
					atlas[index + 0] = value;
					atlas[index + 1] = value;
					atlas[index + 2] = value;
					atlas[index + 3] = value;
				}
			}

			//Store glyphs rect and metrics
			auto& gd = font->descriptions[c];
			gd.x0 = penX;
			gd.y0 = penY;
			gd.x1 = penX + static_cast<int>(bmp.width);
			gd.y1 = penY + static_cast<int>(bmp.rows);
			gd.width = static_cast<int>(bmp.width);
			gd.height = static_cast<int>(bmp.rows);
			gd.advance = (face->glyph->advance.x >> 6);

			//Advance cursor with 1px spacing
			penX += static_cast<int>(bmp.width) + 1;
		}

		//Create texture from atlas
		font->texture = std::make_shared<Texture>(textureWidth, textureHeight, 4, atlas);
		font->size = size;

		//Caches it
		fonts[path][size] = font;

		//Cleanup
		delete[] atlas;
		FT_Done_Face(face);

		return font;
	}
}