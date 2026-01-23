#pragma once

#include "scene/components/ui/UIElementComponent.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <string>

namespace eng
{
	class Font;

	class TextComponent : public UIElementComponent
	{
		COMPONENT(TextComponent)

	public:
		void LoadProperties(const nlohmann::json& json) override;
		void Render(CanvasComponent* canvas) override;

		void SetText(const std::string& newText);
		const std::string& GetText() const;

		void SetColour(const glm::vec4& newColour);
		const glm::vec4& GetColour() const;

		void SetFont(const std::shared_ptr<Font>& newFont);
		void SetFont(const std::string& path, int size);
		const std::shared_ptr<Font>& GetFont() const;

		glm::vec2 GetPivotPos() const;

	private:
		std::string text;
		glm::vec4 colour = glm::vec4(1.0f);
		std::shared_ptr<Font> font;
		
	};
}