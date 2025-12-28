#include "Game.h"
#include "TestObject.h"
#include "TestObjectOrbit.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>




bool Game::Init()
{
    auto& fs = eng::Engine::GetInstance().GetFileSystem();
    auto texture = eng::Texture::Load("textures/brick.png");

    scene = new eng::Scene();
	eng::Engine::GetInstance().SetScene(scene);

    cameraOne = scene->CreateObject("CameraOne");
    cameraOne->AddComponent(new eng::CameraComponent());
    cameraOne->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    cameraOne->AddComponent(new eng::PlayerControllerComponent());

    cameraTwo = scene->CreateObject("CameraTwo");
    cameraTwo->AddComponent(new eng::CameraComponent());
    cameraTwo->SetPosition(glm::vec3(-20.0f, 0.0f, 2.0f));
    cameraTwo->AddComponent(new eng::PlayerControllerComponent());

    scene->SetMainCamera(cameraOne);
    auto material = eng::Material::Load("materials/brick.mat");


	auto mesh = eng::Mesh::CreateCube();

    auto objectA = scene->CreateObject("ObjectA");
    objectA->AddComponent(new eng::MeshComponent(material, mesh));
    objectA->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectB = scene->CreateObject("ObjectB");
    objectB->AddComponent(new eng::MeshComponent(material, mesh));
    objectB->SetPosition(glm::vec3(-10.0f, 2.0f, 2.0f));
    objectB->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectC = scene->CreateObject("ObjectC");
    objectC->AddComponent(new eng::MeshComponent(material, mesh));
    objectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objectC->SetRotation(glm::vec3(1.0f, 0.0f, 1.0f));
    objectC->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

	auto suzanneObj = eng::GameObject::LoadGLTF("models/suzanne/Suzanne.gltf");
	suzanneObj->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

	auto gunObj = eng::GameObject::LoadGLTF("models/weapons/scene.gltf");
	gunObj->SetParent(cameraOne);
	gunObj->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
	gunObj->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    auto light = scene->CreateObject("Light");
    auto lightComp = new eng::LightComponent();
    lightComp->SetColour(glm::vec3(1.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));


    

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