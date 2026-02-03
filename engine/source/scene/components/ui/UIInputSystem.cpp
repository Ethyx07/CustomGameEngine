#include "UIInputSystem.h"
#include "scene/components/ui/CanvasComponent.h"
#include "scene/components/ui/UIElementComponent.h"
#include "Engine.h"
#include <iostream>

#include <GLFW/glfw3.h>

namespace eng
{
	void UIInputSystem::SetActive(bool bIsActive)
	{
		bActive = bIsActive;
	}

	bool UIInputSystem::IsActive() const
	{
		return bActive;
	}

	void UIInputSystem::SetCanvas(CanvasComponent* canvas)
	{
		activeCanvas = canvas;
	}

	CanvasComponent* UIInputSystem::GetCanvas() const
	{
		return activeCanvas;
	}

	void UIInputSystem::Update(float deltaTime)
	{
		if (!bActive || !activeCanvas)
		{
			return;
		}
		auto& input = Engine::GetInstance().GetInputManager();
		//Checks if button is currently pressed, was pressed or was released
		bool bMouseDown = input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
		bool bMousePressed = input.WasMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
		bool bMouseReleased = input.WasMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT);

		auto mousePos = input.GetCurrentMousePosition();
		mousePos.y = Engine::GetInstance().GetGraphicsAPI().GetViewport().height - mousePos.y;

		UIElementComponent* hit = nullptr;

		auto uiElements = CollectUI(activeCanvas);
		for (auto element : uiElements)
		{
			if (element->HitTest(mousePos))
			{
				hit = element;
				break;
			}
		}

		if (hoveredElement != hit)
		{
			if (hoveredElement)
			{
				hoveredElement->OnPointerExit();
			}

			hoveredElement = hit;

			if (hoveredElement)
			{
				hoveredElement->OnPointerEnter();
			}
			pressedElement = nullptr;
		}

		if (!pressedElement)
		{
			if (bMousePressed && hoveredElement)
			{
				pressedElement = hoveredElement;
				pressedElement->OnPointerDown();
			}
		}

		if (bMouseReleased)
		{
			if (pressedElement)
			{
				pressedElement->OnPointerUp();

				if (pressedElement == hoveredElement)
				{
					pressedElement->OnClick();
				}
			}

			pressedElement = nullptr;
		}
	}

	std::vector<UIElementComponent*> UIInputSystem::CollectUI(CanvasComponent* canvas)
	{
		std::vector<UIElementComponent*> result;
		GameObject* canvasObject = canvas->GetOwner();
		const auto& children = canvasObject->GetChildren();

		for (const auto& child : children)
		{
			if (auto component = child->GetComponent<UIElementComponent>())
			{
				canvas->CollectUI(component, result);
			}
		}

		return result;
	}
}