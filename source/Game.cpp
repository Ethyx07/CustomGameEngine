#include "Game.h"
#include "TestObject.h"
#include <string>

bool Game::Init()
{
	scene = new eng::Scene();
	auto camera = scene->CreateObject("Camera");
	camera->AddComponent(new eng::CameraComponent());
	camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	camera->AddComponent(new eng::PlayerControllerComponent());

	scene->SetMainCamera(camera);
	auto obj = scene->CreateObject<TestObject>("TestObject");
	scene->CreateObject<TestObject>("TestObject");
	obj->SetScale(glm::vec3(5.0f, 1.0f, 1.0f));
	obj->SetPosition(glm::vec3(10.0f, 10.0f, 5.0f));
	eng::Engine::GetInstance().SetScene(scene);
	return true;
}

void Game::Update(float deltaTime)
{	
	scene->Update(deltaTime);
}

void Game::Destroy()
{

}