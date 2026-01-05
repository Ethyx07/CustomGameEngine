#pragma once
#include <array>
#include <glm/vec2.hpp>

namespace eng 
{
	class InputManager 
	{

	private:
		InputManager() = default;
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		InputManager& operator = (const InputManager&) = delete;
		InputManager& operator = (InputManager&&) = delete;

	public:
		void SetKeyPressed(int key, bool pressed);
		bool isKeyPressed(int key);

		void SetMouseButtonPressed(int button, bool pressed);
		bool isMouseButtonPressed(int button);

		void SetOldMousePosition(glm::vec2 pos);
		const glm::vec2 GetOldMousePosition() const;

		void SetCurrentMousePosition(glm::vec2 pos);
		const glm::vec2 GetCurrentMousePosition() const;

		void SetMousePositionChanged(bool changed);
		bool IsMousePositionChanged() const;


	private:
		std::array<bool, 256> keys = { false };
		std::array<bool, 16> mouseKeys = { false };
		glm::vec2 oldMousePosition = glm::vec2(0.0f, 0.0f);
		glm::vec2 currentMousePosition = glm::vec2(0.0f, 0.0f);

		bool bMousePositionChanged = false;

		friend class Engine; //Lets engine access private functions/values

	};
}