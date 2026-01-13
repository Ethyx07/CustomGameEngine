#pragma once

#include "Common.h"
#include <vector>

namespace eng
{
	class IContactListener;

	enum class CollisionObjectType
	{
		RigidBody,
		KinematicPlayerController
	};

	class CollisionObject
	{
	public:
		CollisionObjectType GetCollisionObjectType() const;

		void AddContactListener(IContactListener* listener);
		void RemoveContactListener(IContactListener* listener);

	protected:
		void DispatchContactEvent(CollisionObject* obj, const glm::vec3& position, const glm::vec3& normal);

		CollisionObjectType collisionObjectType;
		std::vector<IContactListener*> contactListeners;

		friend class PhysicsManager;

	};

	class IContactListener
	{
	public:
		virtual void OnContact(CollisionObject* obj, const glm::vec3& position, const glm::vec3& normal) = 0;
	};
}