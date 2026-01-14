#pragma once

#include <eng.h>

class JumpPlatform : public eng::GameObject, public eng::IContactListener
{
	GAMEOBJECT(JumpPlatform)
public:
	void Init() override;

	void OnContact(eng::CollisionObject* obj, const glm::vec3& position, const glm::vec3& normal) override;

private:
	eng::AudioComponent* audioComponent = nullptr;
};