#pragma once

#include "graphics/Texture.h"

namespace eng
{
	struct GlyphDescription
	{
		int x0, y0; //Lower left UV coordinates
		int x1, y1; //Upper right UV coordinates

		int width; //Rasterized width
		int height; //Rasterized height

		int advance; //Horizontal advance(distance to next glyph)
	};

	class Font 
	{
	public:
		int GetSize() const;
		const GlyphDescription& GetGlyphDescription(char asciiCode) const;
		const std::shared_ptr<Texture>& GetTexture() const;

	private:
		int size = 0; //Pixel size for the font

		GlyphDescription descriptions[128]; //Keeping it 0-127 as its standard ascii table
		std::shared_ptr<Texture> texture; //The texture atlas

		friend class FontManager; //Needs to be able to fill private fields
	};
}