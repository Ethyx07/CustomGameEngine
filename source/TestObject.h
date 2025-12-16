#pragma once
#include <eng.h>


class TestObject : public eng::GameObject
{
public:
	TestObject();

	void Update(float deltaTime) override;

private:

	bool bToggle = false;
	bool bToggleCooldown = false;
	float toggleCooldown = 2.0f;
	float currentCooldown = 0.0f;

	float fpsCounter = 0;
	float timeSinceLastSecond = 0.0f;
	float time = 0;

};