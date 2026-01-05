#pragma once
#include <eng.h>

class AnimationComponent;

class Player : public eng::GameObject
{
public:
	void Init();
	void Update(float deltaTime) override;

private:
	eng::AnimationComponent* animationComponent = nullptr;
};