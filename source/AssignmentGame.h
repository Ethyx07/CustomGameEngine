#pragma once
#include <eng.h>

class AssignmentGame : public eng::Application
{
public:
	bool Init() override;
	void Update(float deltaTime) override;
	void Destroy() override;

private:
	eng::Material material;
	eng::Material material2;
	std::unique_ptr<eng::Mesh> mesh;
	float offsetX = 0;
	float offsetY = 0;
	bool bToggle = false;
	bool bToggleCooldown = false;
	float toggleCooldown = 2.0f;
	float currentCooldown = 0.0f;

	float fpsCounter = 0;
	float timeSinceLastSecond = 0.0f;
	float time = 0;
};