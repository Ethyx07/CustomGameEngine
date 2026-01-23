#include "font/Font.h"

namespace eng
{
	int Font::GetSize() const
	{
		return size;
	}

	const GlyphDescription& Font::GetGlyphDescription(char asciiCode) const
	{
		return descriptions[static_cast<unsigned char>(asciiCode)];
	}

	const std::shared_ptr<Texture>& Font::GetTexture() const
	{
		return texture;
	}
}