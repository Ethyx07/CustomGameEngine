#include "scene/components/PhysicsComponent.h"
#include "scene/GameObject.h"
#include "Engine.h"

namespace eng
{
	PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& body) 
		: rigidBody(body)
	{
	}

	void PhysicsComponent::LoadProperties(const nlohmann::json& json)
	{

		//Collider Loading
		std::shared_ptr<Collider> collider;
		if (json.contains("collider"))
		{
			const auto& colData = json["collider"];
			std::string type = colData.value("type", "");

			if (type == "box") //Gathers data based on what type of collider it is
			{
				glm::vec3 extents(colData.value("x", 1.0f),
					colData.value("y", 1.0f),
					colData.value("z", 1.0f));
				collider = std::make_shared<BoxCollider>(extents);
			}
			else if (type == "sphere")
			{
				float radius = colData.value("radius", 1.0f);
				collider = std::make_shared<SphereCollider>(radius);
			}
			else if (type == "cylinder")
			{
				float radius = colData.value("radius", 1.0f);
				float height = colData.value("height", 1.0f);
			}
		}

		if (!collider) //If no collider then return since physics comp needs a collider
		{
			return;
		}

		//RigidBody Loading
		std::shared_ptr<RigidBody> rigidBody;
		if (json.contains("body"))
		{
			const auto& bodyData = json["body"]; //Sets rigid body values

			float mass = bodyData.value("mass", 0.0f);
			float friction = bodyData.value("friction", 0.5f);
			std::string typeString = bodyData.value("type", "static"); //Sets value as the type, if no type is defined, static is default
			BodyType bodyType = BodyType::Static;
			if (typeString == "dynamic")
			{
				bodyType = BodyType::Dynamic;
			}
			else if (typeString == "kinematic")
			{
				bodyType = BodyType::Kinematic;
			}

			rigidBody = std::make_shared<RigidBody>(bodyType, collider, mass, friction);

		}

		if (rigidBody)
		{
			SetRigidBody(rigidBody);
			rigidBody->SetOwner(GetOwner());
		}
	}

	void PhysicsComponent::Init()
	{
		if (!rigidBody) //Cant initialise if the physics component doesnt have a rigid body
		{
			return;
		}
		rigidBody->SetOwner(GetOwner());
		const auto pos = owner->GetWorldPosition();
		const auto rot = owner->GetWorldRotation();

		rigidBody->SetPosition(pos);
		rigidBody->SetRotation(rot);

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

	const std::shared_ptr<RigidBody>& PhysicsComponent::GetRigidBody() const
	{
		return rigidBody;
	}
}