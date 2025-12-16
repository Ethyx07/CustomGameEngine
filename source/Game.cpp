#include "Game.h"
#include "TestObject.h"
#include "GLFW/glfw3.h"
#include <string>

bool Game::Init()
{
	scene = new eng::Scene();
	cameraOne = scene->CreateObject("CameraOne");
	cameraOne->AddComponent(new eng::CameraComponent());
	cameraOne->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	cameraOne->AddComponent(new eng::PlayerControllerComponent());

	scene->SetMainCamera(cameraOne);

	cameraTwo = scene->CreateObject("CameraTwo");
	cameraTwo->AddComponent(new eng::CameraComponent());
	cameraTwo->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	cameraTwo->AddComponent(new eng::PlayerControllerComponent());
	cameraTwo->GetComponent<eng::PlayerControllerComponent>()->bActive = false;

	auto obj = scene->CreateObject<TestObject>("TestObject");
	scene->CreateObject<TestObject>("TestObject");
	obj->SetScale(glm::vec3(5.0f, 1.0f, 1.0f));
	obj->SetPosition(glm::vec3(10.0f, 10.0f, 5.0f));

	auto obj2 = scene->CreateObject<TestObject>("TestObject");
	scene->CreateObject<TestObject>("TestObject");
	obj2->SetScale(glm::vec3(5.0f, 1.0f, 1.0f));
	obj2->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	eng::Engine::GetInstance().SetScene(scene);
	return true;
}

void Game::Update(float deltaTime)
{	
	scene->Update(deltaTime);
	auto& inputManager = eng::Engine::GetInstance().GetInputManager();
	if (bCooldown)
	{
		currentCooldownTimer += deltaTime;
		if (currentCooldownTimer >= cooldownTimer)
		{
			bCooldown = false;
			currentCooldownTimer = 0.0f;
		}
		
	}
	else
	{
		if (inputManager.isKeyPressed(GLFW_KEY_P))
		{
			if (bCameraOne)
			{
				scene->SetMainCamera(cameraTwo);
				bCameraOne = false;
				cameraOne->GetComponent<eng::PlayerControllerComponent>()->bActive = false;
				cameraTwo->GetComponent<eng::PlayerControllerComponent>()->bActive = true;
			}
			else
			{
				scene->SetMainCamera(cameraOne);
				bCameraOne = true;
				cameraOne->GetComponent<eng::PlayerControllerComponent>()->bActive = true;
				cameraTwo->GetComponent<eng::PlayerControllerComponent>()->bActive = false;
			}
			bCooldown = true;
		}
	}
	
}

void Game::Destroy()
{

}