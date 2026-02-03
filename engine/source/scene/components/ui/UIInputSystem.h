#pragma once

#include <vector>

namespace eng
{
	class UIElementComponent;
	class CanvasComponent;

	class UIInputSystem
	{
	public:
		void SetActive(bool bIsActive);
		bool IsActive() const;
		void SetCanvas(CanvasComponent* canvas);
		CanvasComponent* GetCanvas() const;
		void Update(float deltaTime);

		std::vector<UIElementComponent*> CollectUI(CanvasComponent* canvas);

	private:
		bool bActive = false;
		CanvasComponent* activeCanvas = nullptr;
		UIElementComponent* hoveredElement = nullptr;
		UIElementComponent* pressedElement = nullptr;
	};
}