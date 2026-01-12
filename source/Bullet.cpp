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