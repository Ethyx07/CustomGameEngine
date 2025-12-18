#pragma once
#include "scene/Component.h"

namespace eng
{
	class PlayerControllerComponent : public Component
	{
		COMPONENT(PlayerControllerComponent)
	public: 
		void Update(float deltaTime) override;
		void SetIsActive(bool bActive);
		bool GetIsActive();

	private:
		float sensitivity = 1.5f;
		float moveSpeed = 2.0f;
		bool bIsActive = true;

	};
}