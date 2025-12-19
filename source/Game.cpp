#include "Game.h"
#include "TestObject.h"
#include "TestObjectOrbit.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


bool Game::Init()
{
	auto& fs = eng::Engine::GetInstance().GetFileSystem();
	auto path = fs.GetAssetsFolder() / "brick.png";

	int width, height, channels;
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
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
	cameraTwo->GetComponent<eng::PlayerControllerComponent>()->SetIsActive(false);

	auto obj = scene->CreateObject<TestObject>("TestObject");
	obj->SetScale(glm::vec3(5.0f, 1.0f, 1.0f));
	obj->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	
	auto obj2 = scene->CreateObject<TestObject>("TestObject2");
	obj2->SetScale(glm::vec3(2.0f, 4.0f, 0.5f));
	obj2->SetPosition(glm::vec3(10.0f, 10.0f, -10.0f));

	auto obj3 = scene->CreateObject<TestObjectOrbit>("TestObject3");
	obj3->SetScale(glm::vec3(0.5f, 0.25f, 2.0f));
	obj3->SetPosition(glm::vec3(0.5f, 1.0f, 1.0f));

	scene->SetParent(obj3, obj2);
	//scene->SetParent(obj2, obj);
	eng::Engine::GetInstance().SetScene(scene);
	return true;
}

void Game::Update(float deltaTime)
{	
	scene->Update(deltaTime);

	time += deltaTime;
	fpsCounter += 1;
	timeSinceLastSecond += deltaTime; //Basic fps counter that adds the fps each update until its been a second and then prints and resets
	if (timeSinceLastSecond >= 1.0f) {
		std::cout << "FPS: " << fpsCounter << std::endl;
		fpsCounter = 0;
		timeSinceLastSecond = 0;
	}

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
				cameraOne->GetComponent<eng::PlayerControllerComponent>()->SetIsActive(false);
				cameraTwo->GetComponent<eng::PlayerControllerComponent>()->SetIsActive(true);
			}
			else
			{
				scene->SetMainCamera(cameraOne);
				bCameraOne = true;
				cameraOne->GetComponent<eng::PlayerControllerComponent>()->SetIsActive(true);
				cameraTwo->GetComponent<eng::PlayerControllerComponent>()->SetIsActive(false);
			}
			bCooldown = true;
		}
	}
	
}

void Game::Destroy()
{

}