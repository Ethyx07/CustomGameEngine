#pragma once
#include <eng.h>


class TestObject : public eng::GameObject
{
public:
	TestObject();

	void Update(float deltaTime) override;

private:
	eng::Material material;
	eng::Material material2;
	std::shared_ptr<eng::Mesh> mesh;

	bool bToggle = false;
	bool bToggleCooldown = false;
	float toggleCooldown = 2.0f;
	float currentCooldown = 0.0f;

	float fpsCounter = 0;
	float timeSinceLastSecond = 0.0f;
	float time = 0;
};