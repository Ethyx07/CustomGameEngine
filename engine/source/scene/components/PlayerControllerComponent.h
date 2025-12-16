#pragma once
#include "scene/Component.h"

namespace eng
{
	class PlayerControllerComponent : public Component
	{
		COMPONENT(PlayerControllerComponent)
	public: 
		void Update(float deltaTime) override;

	private:
		float sensitivity = 1.5f;
		float moveSpeed = 2.0f;
	};
}