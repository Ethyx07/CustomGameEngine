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
		auto rigidBody = static_cast<eng::RigidBody*>(obj); //How to get kinematic controller from player
		if (controller)
		{
			controller->Jump(glm::vec3(0.0f, 20.0f, 0.0f));
		}
	}
}
