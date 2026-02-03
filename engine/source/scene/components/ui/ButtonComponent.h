#pragma once
#include "scene/components/ui/UIElementComponent.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <functional>

namespace eng
{
	class ButtonComponent : public UIElementComponent
	{
		COMPONENT(ButtonComponent)

	public:
		void LoadProperties(const nlohmann::json& json) override;
		void Render(CanvasComponent* canvas) override;

		bool HitTest(const glm::vec2& pos) const override;
		void OnPointerEnter() override;
		void OnPointerExit() override;
		void OnPointerUp() override;
		void OnPointerDown() override;
		void OnClick() override;

		void SetRect(const glm::vec2& newRect);
		const glm::vec2& GetRect() const;
		void SetColour(const glm::vec4& newColour);
		const glm::vec4& GetColour() const;

		void SetHoveredColour(const glm::vec4& newColour);
		const glm::vec4& GetHoveredColour() const;

		void SetPressedColour(const glm::vec4& newColour);
		const glm::vec4& GetPressedColour() const;

		std::function<void()> onClick;

	private:
		glm::vec2 rect;
		glm::vec4 colour = glm::vec4(1.0f);

		glm::vec4 hoveredColour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		glm::vec4 pressedColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		const glm::vec4* currentColour = &colour;
	};
}