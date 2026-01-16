#pragma once

#include "scene/components/ui/UIElementComponent.h"
#include <string>

namespace eng
{
	class TextComponent : public UIElementComponent
	{
		COMPONENT(TextComponent)

	public:
		void Render(CanvasComponent* canvas) override;

		void SetText(const std::string& newText);
		const std::string& GetText() const;

	private:
		std::string text;
		
	};
}