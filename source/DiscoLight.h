#pragma once
#include <eng.h>

class DiscoLight : public eng::LightComponent
{
	COMPONENT(DiscoLight)
public:
	void Update(float deltaTime) override;
	void SetDiscoMode(bool disco);
	bool GetDiscoMode();

private:
	float colourTime = 0.0f;
	bool bDiscoMode = false;

	bool bCoolingDown = false;
	float cooldownTime = 2.0f;
	float currentTime = 0.0f;
};