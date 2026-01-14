#include "Bullet.h"


void Bullet::Update(float deltaTime)
{
	eng::GameObject::Update(deltaTime);

	lifetime -= deltaTime;

	if (lifetime <= 0.0f) //Object is destroyed after lifetime reaches 0
	{
		MarkForDestroy();
	}

}

void Bullet::OnContact(eng::CollisionObject* obj, const glm::vec3& position, const glm::vec3& normal)
{
	if (obj->GetCollisionObjectType() == eng::CollisionObjectType::RigidBody)
	{
		auto rigidBody = static_cast<eng::RigidBody*>(obj); //Passes it to rigidbody
		if (rigidBody)
		{
			auto hitObj = rigidBody->GetOwner();
			if (hitObj && hitObj->GetObjectType() == eng::ObjectType::DESTRUCTABLE) //Only destroys destructables
			{
				hitObj->MarkForDestroy();
				this->MarkForDestroy();
			}
		}
	}
}
