#include "scene/components/ui/TextComponent.h"
#include "scene/components/ui/CanvasComponent.h"
#include "Engine.h"

namespace eng
{
	void TextComponent::LoadProperties(const nlohmann::json& json)
	{
		const std::string text = json.value("text", "");
		SetText(text);

		if (json.contains("font")) //Checks if there is a custom font to the text component
		{
			const auto& fontData = json["font"];
			std::string fontPath = fontData.value("path", ""); //Gets the path to the font
			int fontSize = fontData.value("size", 12); //Either returns size value or defaults to 12
			if (!fontPath.empty())
			{
				SetFont(fontPath, fontSize);
			}
		}

		if (json.contains("colour"))
		{
			const auto& colourData = json["colour"];
			glm::vec4 colour(
				colourData.value("r", 1.0f),
				colourData.value("g", 1.0f),
				colourData.value("b", 1.0f),
				colourData.value("a", 1.0f)
			); //Sets the colour vec based on the data in the json
			SetColour(colour); //Updates colour based on json data
		}
	}

	void TextComponent::Render(CanvasComponent* canvas)
	{
		if (text.empty() || !font || !canvas) //Ensures that render can only occur if there is a valid canvas, font and text isnt empty
		{
			return;
		}
		int width = font->GetTexture()->GetWidth();
		int height = font->GetTexture()->GetHeight();

		auto pos = GetPivotPos();
		float xOrigin = pos.x;
		float yOrigin = pos.y;

		for (const auto c : text)
		{
			const auto& desc = font->GetGlyphDescription(c);

			float x1 = static_cast<float>(xOrigin);
			float y1 = static_cast<float>(yOrigin);
			float x2 = static_cast<float>(xOrigin + desc.width);
			float y2 = static_cast<float>(yOrigin + desc.height);

			float u1 = static_cast<float>(desc.x0) / static_cast<float>(width);
			float v1 = static_cast<float>(desc.y0) / static_cast<float>(height);
			float u2 = static_cast<float>(desc.x1) / static_cast<float>(width);
			float v2 = static_cast<float>(desc.y1) / static_cast<float>(height);

			xOrigin += desc.advance;

			canvas->DrawRect(
			glm::vec2(x1,y1), glm::vec2(x2,y2),
				glm::vec2(u1,v2), glm::vec2(u2,v1),
				font->GetTexture().get(), colour
			);
		}
	}

	void TextComponent::SetText(const std::string& newText) //Updates text with new string
	{
		text = newText;
	}

	const std::string& TextComponent::GetText() const //Returns text components current text string
	{
		return text;
	}

	void TextComponent::SetColour(const glm::vec4& newColour)
	{
		colour = newColour;
	}

	const glm::vec4& TextComponent::GetColour() const
	{
		return colour;
	}

	void TextComponent::SetFont(const std::shared_ptr<Font>& newFont)
	{
		font = newFont;
	}

	void TextComponent::SetFont(const std::string& path, int size)
	{
		font = Engine::GetInstance().GetFontManager().GetFont(path, size);
	}

	const std::shared_ptr<Font>& TextComponent::GetFont() const
	{
		return font;
	}

	glm::vec2 TextComponent::GetPivotPos() const
	{
		auto pos = owner->GetWorldPosition2D();

		glm::vec2 rect(0.0f);
		for (const char c : text)
		{
			const auto& desc = font->GetGlyphDescription(c);
			rect.x += static_cast<float>(desc.advance);
			rect.y = std::max(rect.y, static_cast<float>(desc.height));
		}

		pos.x -= std::round(rect.x * pivot.x);
		pos.y -= std::round(rect.y * pivot.y);
		return pos;
	}
}