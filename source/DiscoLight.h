#pragma once
#include <eng.h>

class DiscoLight : public eng::LightComponent
{
	COMPONENT(DiscoLight)
public:
	void Update(float deltaTime) override;

private:
	float colourTime = 0.0f;
};