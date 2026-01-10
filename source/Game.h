#pragma once
#include "source/scene/Scene.h"
#include <eng.h>
#include <memory>

class Game : public eng::Application
{
public:
	void RegisterTypes() override;
	bool Init() override;
	void Update(float deltaTime) override;
	void Destroy() override;

private:
	std::shared_ptr<eng::Scene> scene = nullptr;

	bool bCameraOne = true;
	bool bCooldown = false;

	float cooldownTimer = 2.0f;
	float currentCooldownTimer = 0.0f;

	float fpsCounter = 0;
	float timeSinceLastSecond = 0.0f;
	float time = 0;
};