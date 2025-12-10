#include "Game.h"
#include <iostream>

bool Game::Init()
{
	return true;
}

void Game::Update(float deltaTime)
{	

	std::cout << "Current Delta: " << deltaTime << std::endl;
}

void Game::Destroy()
{

}