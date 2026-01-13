#pragma once
#include <eng.h>

class Bullet : public eng::GameObject, public eng::IContactListener
{
	GAMEOBJECT(Bullet)
public:
	void Update(float deltaTime) override;

	void OnContact(eng::CollisionObject* obj, const glm::vec3& position, const glm::vec3& normal) override;
private:
	float lifetime = 2.0f;
};