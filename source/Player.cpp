#include "Player.h"
#include <GLFW/glfw3.h>

/*	Basic Template player class used for testing the different character implementations.Will be altered to a more
	template/reusable version upon engine completion */

void Player::Init() 
{
	AddComponent(new eng::CameraComponent());
	SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	AddComponent(new eng::PlayerControllerComponent());

	auto gun = eng::GameObject::LoadGLTF("models/weapons/scene.gltf");
	gun->SetParent(this);
	gun->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
	gun->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

	if (auto anim = gun->GetComponent<eng::AnimationComponent>())
	{
		if (auto bullet = gun->FindChildByName("bullet_33"))
		{
			bullet->SetActive(false);
		}
		if (auto fire = gun->FindChildByName("BOOM_35"))
		{
			fire->SetActive(false);
		}

	}

	animationComponent = gun->GetComponent<eng::AnimationComponent>();
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
		}
	}
}