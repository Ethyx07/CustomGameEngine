#include "Player.h"

#include "Bullet.h"
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

			auto bullet = scene->CreateObject<Bullet>("Bullet");
			auto material = eng::Material::Load("materials/suzanne.mat");
			auto mesh = eng::Mesh::CreateSphere(0.2f, 32, 32);
			bullet->AddComponent(new eng::MeshComponent(material, mesh));
			glm::vec3 pos = glm::vec3(0.0f);

			if (auto child = FindChildByName("BOOM_35"))
			{
				pos = child->GetWorldPosition();
			}
			bullet->SetPosition(pos + rotation * glm::vec3(-0.2f, 0.2f, -1.75f));

			//To create a physics component it goes: Create collider -> Create Rigidbody with collider value -> Create PhysicsComponent with rigidbody as value
			auto collider = std::make_shared<eng::SphereCollider>(0.2f);
			auto rigidBody = std::make_shared<eng::RigidBody>(
				eng::BodyType::Dynamic, collider, 10.0f, 0.1f);

			bullet->AddComponent(new eng::PhysicsComponent(rigidBody));

			glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
			rigidBody->ApplyImpulse(front * 500.0f);
		}
	}
	if (input.isKeyPressed(GLFW_KEY_SPACE))
	{
		if (audioComponent && !audioComponent->isPlaying("jump"))
		{
			audioComponent->Play("jump");
		}
	}

	if (input.isKeyPressed(GLFW_KEY_Q))
	{
		if (playerControllerComponent)
		{
			playerControllerComponent->SetMoveSpeed(50.0f);
		}
	}
	else if (playerControllerComponent->GetMoveSpeed() != 10.0f)
	{
		playerControllerComponent->SetMoveSpeed(10.0f);
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