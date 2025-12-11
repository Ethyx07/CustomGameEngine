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
		if (key < 0 || key >= static_cast<int>(keys.size()))
		{
			return false;
		}
		return keys[key];
	}
}