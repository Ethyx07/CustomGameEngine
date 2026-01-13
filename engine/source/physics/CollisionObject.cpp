#include "physics/CollisionObject.h"

namespace eng
{
	CollisionObjectType CollisionObject::GetCollisionObjectType() const
	{
		return collisionObjectType;
	}

	void CollisionObject::AddContactListener(IContactListener* listener)
	{
		contactListeners.push_back(listener); //Adds listener to contact listener list
	}

	void CollisionObject::RemoveContactListener(IContactListener* listener) //Checks if listener is in contactListener vector. If it is, erase it
	{
		auto iterator = std::find(contactListeners.begin(), contactListeners.end(), listener);
		if (iterator != contactListeners.end())
		{
			contactListeners.erase(iterator);
		}
	}

	void CollisionObject::DispatchContactEvent(CollisionObject* obj, const glm::vec3& position, const glm::vec3& normal)
	{
		for (auto listener : contactListeners) //Checks for valid listener and runs on contact logic
		{
			if (listener)
			{
				listener->OnContact(obj, position, normal);
			}
		}
	}
}