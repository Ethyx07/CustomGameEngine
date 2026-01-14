#include "JumpPlatform.h"

void JumpPlatform::Init()
{
	audioComponent = GetComponent<eng::AudioComponent>();
	auto physics = GetComponent<eng::PhysicsComponent>();
	if (physics) //Gets physics component
	{
		auto rigidBody = physics->GetRigidBody();
		if (rigidBody) //Gets rigidbody comp and adds it to the contact listeners
		{
			rigidBody->AddContactListener(this);
		}
	}
}

void JumpPlatform::OnContact(eng::CollisionObject* obj, const glm::vec3& position, const glm::vec3& normal)
{
	if (obj->GetCollisionObjectType() == eng::CollisionObjectType::KinematicPlayerController)
	{
		auto controller = static_cast<eng::KinematicCharacterController*>(obj); //How to get kinematic controller from player
		if (controller)
		{
			controller->Jump(glm::vec3(0.0f, 20.0f, 0.0f));
			if (audioComponent)
			{
				audioComponent->Play("boing", false);
			}
		}
	}
}