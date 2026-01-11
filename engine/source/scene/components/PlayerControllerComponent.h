#pragma once
#include "scene/Component.h"
#include "physics/KinematicCharacterController.h"
#include <memory>

namespace eng
{
	class PlayerControllerComponent : public Component
	{
		COMPONENT(PlayerControllerComponent)

	public: 
		void Init() override;
		void Update(float deltaTime) override;

		void SetIsActive(bool bActive);
		bool GetIsActive();
		bool OnGround() const;

	private:
		float sensitivity = 10.0f;
		float moveSpeed = 20.0f;
		bool bIsActive = true;

		float curXRot = 0.0f;
		float curYRot = 0.0f;

		std::unique_ptr<KinematicCharacterController> kinematicController;

	};
}