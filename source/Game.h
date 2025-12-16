#pragma once
#include <eng.h>
#include <memory>

class Game : public eng::Application
{
public:
	bool Init() override;
	void Update(float deltaTime) override;
	void Destroy() override;

private:
	eng::Scene* scene = nullptr;
	eng::GameObject* cameraOne = nullptr;
	eng::GameObject* cameraTwo = nullptr;

	bool bCameraOne = true;
	bool bCooldown = false;

	float cooldownTimer = 2.0f;
	float currentCooldownTimer = 0.0f;
};