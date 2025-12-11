#include <Game.h>
#include <eng.h>


int main() 
{
	Game* game = new Game(); //Game application used in the engine
	eng::Engine& engine = eng::Engine::GetInstance();

	engine.SetApplication(game); //Sets game as engine's application

	if (engine.Init(1280, 720)) //Ensures engine is initialised before running
	{
		engine.Run(); //Runs if engine is initialised
	}

	engine.Destroy(); //Destroys engine if init fails or engine.run finishes
	return 0;
};