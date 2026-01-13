#pragma once
#include <eng.h>

class Bullet : public eng::GameObject, public eng::IContactListener
{
	GAMEOBJECT(Bullet)
public:
	void Update(float deltaTime) override;

private:
	float lifetime = 2.0f;
};