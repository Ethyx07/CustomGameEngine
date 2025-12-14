#include "Game.h"
#include "TestObject.h"
#include <string>

bool Game::Init()
{
	scene.CreateObject<TestObject>("TestObject");
	return true;
}

void Game::Update(float deltaTime)
{	
	scene.Update(deltaTime);
}

void Game::Destroy()
{

}