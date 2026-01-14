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
	scene = eng::Scene::Load("scenes/level01.sc");
	eng::Engine::GetInstance().SetScene(scene.get());


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