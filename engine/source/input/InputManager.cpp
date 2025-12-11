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
}