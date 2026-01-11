#include "Player.h"
#include <GLFW/glfw3.h>

/*	Basic Template player class used for testing the different character implementations.Will be altered to a more
	template/reusable version upon engine completion */

void Player::Init() 
{
	audioComponent = GetComponent<eng::AudioComponent>();
	playerControllerComponent = GetComponent<eng::PlayerControllerComponent>();

	if (auto gun = FindChildByName("Gun"))
	{
		animationComponent = gun->GetComponent<eng::AnimationComponent>();
	}

	if (auto bullet = FindChildByName("bullet_33"))
	{
		bullet->SetActive(false);
	}
	if (auto fire = FindChildByName("BOOM_35"))
	{
		fire->SetActive(false);
	}
	
}

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	auto& input = eng::Engine::GetInstance().GetInputManager();
	if (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (animationComponent && !animationComponent->IsPlaying())
		{
			animationComponent->Play("shoot", false);

			if (audioComponent)
			{
				if (audioComponent->isPlaying("shoot"))
				{
					audioComponent->Stop("shoot");
				}
				audioComponent->Play("shoot");
			}
		}
	}
	if (input.isKeyPressed(GLFW_KEY_SPACE))
	{
		if (audioComponent && !audioComponent->isPlaying("jump"))
		{
			audioComponent->Play("jump");
		}
	}

	bool walking = input.isKeyPressed(GLFW_KEY_W) ||
		input.isKeyPressed(GLFW_KEY_A) ||
		input.isKeyPressed(GLFW_KEY_S) ||
		input.isKeyPressed(GLFW_KEY_D);

	if (walking && playerControllerComponent && playerControllerComponent->OnGround())
	{
		if (audioComponent && !audioComponent->isPlaying("step"))
		{
			audioComponent->Play("step");
		}
	}
	else
	{
		if (audioComponent && audioComponent->isPlaying("step"))
		{
			audioComponent->Stop("step");
		}
	}
}