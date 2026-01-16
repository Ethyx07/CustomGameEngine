#include "scene/components/ui/TextComponent.h"

namespace eng
{
	void TextComponent::Render(CanvasComponent* canvas)
	{

	}

	void TextComponent::SetText(const std::string& newText) //Updates text with new string
	{
		text = newText;
	}

	const std::string& TextComponent::GetText() const //Returns text components current text string
	{
		return text;
	}
}