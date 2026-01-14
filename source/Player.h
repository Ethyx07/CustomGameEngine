#pragma once
#include <eng.h>

class AnimationComponent;

class Player : public eng::GameObject
{
	GAMEOBJECT(Player);
public:
	Player() = default;
	void Init();
	void Update(float deltaTime) override;

private:
	eng::AnimationComponent* animationComponent = nullptr;
	eng::AnimationComponent* cubeAnimationComponent = nullptr;

	eng::AudioComponent* audioComponent = nullptr;
	eng::PlayerControllerComponent* playerControllerComponent = nullptr;
};