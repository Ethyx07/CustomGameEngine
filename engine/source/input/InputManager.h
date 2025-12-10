#pragma once
#include <array>

namespace eng 
{
	class InputManager 
	{
		friend class Engine; //Lets engine access private functions/values

	private:
		InputManager() = default;
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		InputManager& operator = (const InputManager&) = delete;
		InputManager& operator = (InputManager&&) = delete;

	public:
		void SetKeyPressed(int key, bool pressed);
		bool isKeyPressed(int key);

	private:
		std::array<bool, 256> keys = { false };

	};
}