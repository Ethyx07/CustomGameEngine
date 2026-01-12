#pragma once
#include <eng.h>

class Bullet : public eng::GameObject
{
	GAMEOBJECT(Bullet)
public:
	void Update(float deltaTime) override;

private:
	float lifetime = 25.0f;
};