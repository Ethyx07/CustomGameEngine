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

	scene->CreateObject<TestObject>("TestObject");

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