#include "ButtonComponent.h"
#include "scene/components/ui/CanvasComponent.h"
#include "scene/GameObject.h"

namespace eng
{
	void ButtonComponent::LoadProperties(const nlohmann::json& json)
	{
		if (json.contains("rect"))
		{
			auto& rectObj = json["rect"];
			SetRect(glm::vec2(
				rectObj.value("x", 1.0f),
				rectObj.value("y", 1.0f)
			));
		}
		if (json.contains("colour"))
		{
			auto colourObj = json["colour"];
			SetColour(glm::vec4(
				colourObj.value("r", 1.0f),
				colourObj.value("g", 1.0f),
				colourObj.value("b", 1.0f),
				colourObj.value("a", 1.0f)
			));
		}

		if (json.contains("hovered"))
		{
			auto& hoveredObj = json["hovered"];
			SetPressedColour(glm::vec4(
				hoveredObj.value("r", 0.8f),
				hoveredObj.value("g", 0.8f),
				hoveredObj.value("b", 0.8f),
				hoveredObj.value("a", 1.0f)
			));
		}

		if (json.contains("pressed"))
		{
			auto& pressedObj = json["pressed"];
			SetPressedColour(glm::vec4(
				pressedObj.value("r", 0.2f),
				pressedObj.value("g", 0.2f),
				pressedObj.value("b", 0.2f),
				pressedObj.value("a", 1.0f)
			));
		}
	}

	void ButtonComponent::Render(CanvasComponent* canvas)
	{
		auto pos = owner->GetWorldPosition2D();
		pos.x -= rect.x * pivot.x;
		pos.y -= rect.y * pivot.y;

		canvas->DrawRect(pos, pos + rect, *currentColour);
	}

	void ButtonComponent::SetRect(const glm::vec2& newRect)
	{
		rect = newRect;
	}

	const glm::vec2& ButtonComponent::GetRect() const
	{
		return rect;
	}

	void ButtonComponent::SetColour(const glm::vec4& newColour)
	{
		colour = newColour;
	}

	const glm::vec4& ButtonComponent::GetColour() const
	{
		return colour;
	}

	void ButtonComponent::SetHoveredColour(const glm::vec4& newColour)
	{
		hoveredColour = newColour;
	}

	const glm::vec4& ButtonComponent::GetHoveredColour() const
	{
		return hoveredColour;
	}

	void ButtonComponent::SetPressedColour(const glm::vec4& newColour)
	{
		pressedColour = newColour;
	}

	const glm::vec4& ButtonComponent::GetPressedColour() const
	{
		return pressedColour;
	}

	bool ButtonComponent::HitTest(const glm::vec2& pos) const
	{
		auto ownerPos = owner->GetWorldPosition2D();

		//Gets the x,y positions for the button. Creates an area that the button is in. If mouse is within that area, hit = true
		float x1 = ownerPos.x - (rect.x * pivot.x); 
		float y1 = ownerPos.y - (rect.y * pivot.y);
		float x2 = x1 + rect.x;
		float y2 = y1 + rect.y;

		return (x1 <= pos.x && x2 >= pos.x &&
				y1 <= pos.y && y2 >= pos.y);
	}

	void ButtonComponent::OnPointerEnter() //When player mouse enters button area, current colour is set to hover colour
	{
		currentColour = &hoveredColour;
	}

	void ButtonComponent::OnPointerExit()//When player mouse exits button area, current colour is set to default colour
	{
		currentColour = &colour;
	}

	void ButtonComponent::OnPointerUp() //When mouse is released, sets to hovered colour (mouse would still be over button)
	{
		currentColour = &hoveredColour;
	}

	void ButtonComponent::OnPointerDown() //When mouse is pressed, sets to pressed colour
	{
		currentColour = &pressedColour;
	}

	void ButtonComponent::OnClick()
	{
		if (onClick)
		{
			onClick();
		}
	}
}