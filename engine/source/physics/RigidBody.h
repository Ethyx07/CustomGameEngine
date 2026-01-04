#pragma once
#include "physics/Collider.h"

#include <memory>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>


class btRigidBody;

namespace eng
{
	enum class BodyType
	{
		Static, //Immovable
		Dynamic, //Moves when hit with something
		Kinematic //Player controlled/scripted movements
	};

	

	class RigidBody
	{
	public:
		RigidBody(BodyType bodyType, const std::shared_ptr<Collider>& bodyCollider, float bodyMass, float bodyFriction);
		~RigidBody();

		btRigidBody* GetBody();
		BodyType GetType();
		
		void SetAddedToWorld(bool added);
		bool IsAddedToWorld();

		void SetPosition(const glm::vec3& pos);
		void SetRotation(const glm::quat& rot);
		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;

	private:
		BodyType type = BodyType::Static;
		std::shared_ptr<Collider> collider;
		float mass = 0.0f;
		float friction = 0.0f;
		bool bIsAddedToWorld = false;

		std::unique_ptr<btRigidBody> body;
	};
}