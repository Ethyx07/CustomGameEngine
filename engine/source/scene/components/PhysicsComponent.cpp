#include "scene/components/PhysicsComponent.h"
#include "scene/GameObject.h"
#include "Engine.h"

namespace eng
{
	PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& body) 
		: rigidBody(body)
	{
	}

	void PhysicsComponent::Init()
	{
		if (!rigidBody) //Cant initialise if the physics component doesnt have a rigid body
		{
			return;
		}

		const auto pos = owner->GetWorldPosition();
		const auto rot = owner->GetWorldRotation();

		owner->SetWorldPosition(rigidBody->GetPosition());
		owner->SetWorldRotation(rigidBody->GetRotation());

		Engine::GetInstance().GetPhysicsManager().AddRigidBody(rigidBody.get());
	}

	void PhysicsComponent::Update(float deltaTime)
	{
		if (!rigidBody)
		{
			return;
		}

		if (rigidBody->GetType() == BodyType::Dynamic) //Only dynamic types are updated with physics not static or kinematic
		{
			owner->SetPosition(rigidBody->GetPosition());
			owner->SetRotation(rigidBody->GetRotation());
		}
	}

	void PhysicsComponent::SetRigidBody(const std::shared_ptr<RigidBody>& body)
	{
		rigidBody = body;
	}
}