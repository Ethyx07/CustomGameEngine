#include "input/InputManager.h"


namespace eng 
{
	void InputManager::SetKeyPressed(int key, bool pressed)
	{
		if (key < 0 || key >= static_cast<int>(keys.size())) 
		{
			return;
		}
		keys[key] = pressed;
	}

	bool InputManager::isKeyPressed(int key)
	{
		if (key < 0 || key >= static_cast<int>(keys.size())) //Returns false as this key is outside our bounds
		{
			return false;
		}
		return keys[key];
	}

	void InputManager::SetMouseButtonPressed(int button, bool pressed)
	{
		if (button < 0 || button >= static_cast<int>(mouseKeys.size()))
		{
			return;
		}
		mouseKeys[button] = pressed;
	}

	bool InputManager::isMouseButtonPressed(int button)
	{
		if (button < 0 || button >= static_cast<int>(mouseKeys.size()))
		{
			return false;
		}
		return mouseKeys[button];
	}

	void InputManager::SetOldMousePosition(glm::vec2 pos)
	{
		oldMousePosition = pos;
	}

	const glm::vec2 InputManager::GetOldMousePosition() const
	{
		return oldMousePosition;
	}


	void InputManager::SetCurrentMousePosition(glm::vec2 pos)
	{
		currentMousePosition = pos;
	}

	const glm::vec2 InputManager::GetCurrentMousePosition() const
	{
		return currentMousePosition;
	}

	void InputManager::SetMousePositionChanged(bool changed)
	{
		bMousePositionChanged = changed;
	}

	bool InputManager::IsMousePositionChanged() const
	{
		return bMousePositionChanged;
	}

	void InputManager::SetMouseButtonWasPressed(int button, bool pressed)
	{
		if (button < 0 || button >= static_cast<int>(mouseKeyPressed.size()))
		{
			return;
		}
		mouseKeyPressed[button] = pressed;
	}

	bool InputManager::WasMouseButtonPressed(int button) const
	{
		if (button < 0 || button >= static_cast<int>(mouseKeyPressed.size()))
		{
			return false;
		}
		return mouseKeyPressed[button];
	}

	void InputManager::SetMouseButtonWasReleased(int button, bool released)
	{
		if (button < 0 || button >= static_cast<int>(mouseKeyReleased.size()))
		{
			return;
		}
		mouseKeyReleased[button] = released;
	}

	bool InputManager::WasMouseButtonReleased(int button) const
	{
		if (button < 0 || button >= static_cast<int>(mouseKeyReleased.size()))
		{
			return false;
		}
		return mouseKeyReleased[button];
	}

	void InputManager::ClearStates()
	{
		SetMousePositionChanged(false);
		for (auto& key : mouseKeyPressed)
		{
			SetMouseButtonWasPressed(key, false);
		}
		for (auto& key : mouseKeyReleased)
		{
			SetMouseButtonWasReleased(key, false);
		}
	}
}