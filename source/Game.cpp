#include "Game.h"
#include "TestObject.h"
#include "Player.h"
#include "JumpPlatform.h"
#include "DiscoLight.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>


void Game::RegisterTypes()
{
	Player::Register();
	JumpPlatform::Register();
	DiscoLight::Register();
}

bool Game::Init()
{
	/*scene = eng::Scene::Load("scenes/level01.sc");
	eng::Engine::GetInstance().SetScene(scene.get());*/
	scene = std::make_shared<eng::Scene>();
	eng::Engine::GetInstance().SetScene(scene.get());

	auto sprite = scene->CreateObject("Sprite");
	auto spriteComponent = new eng::SpriteComponent();

	auto texture = eng::Texture::Load("textures/duck.png");
	spriteComponent->SetTexture(texture);

	sprite->AddComponent(spriteComponent);
	sprite->SetPosition2D(glm::vec2(500, 500));
	spriteComponent->SetSize(glm::vec2(64, 64));
	spriteComponent->SetUpperRightUV(glm::vec2(1.0f, 1.0f));
	sprite->SetRotation2D(glm::radians(90.0f));
	sprite->SetScale2D(glm::vec2(5.0f));

	auto camera = scene->CreateObject("Camera");
	auto cameraComponent = new eng::CameraComponent();
	camera->AddComponent(cameraComponent);
	scene->SetMainCamera(camera);


    return true;
}

void Game::Update(float deltaTime)
{	
	scene.get()->Update(deltaTime);

	time += deltaTime;
	fpsCounter += 1;
	timeSinceLastSecond += deltaTime; //Basic fps counter that adds the fps each update until its been a second and then prints and resets
	if (timeSinceLastSecond >= 1.0f) {
		std::cout << "FPS: " << fpsCounter << std::endl;
		fpsCounter = 0;
		timeSinceLastSecond = 0;
	}
}

void Game::Destroy()
{

}