#include <Game.h>
#include <AssignmentGame.h>
#include <eng.h>


int main() 
{
	AssignmentGame* game = new AssignmentGame(); //Game application used in the engine
	eng::Engine& engine = eng::Engine::GetInstance();

	engine.SetApplication(game); //Sets game as engine's application

	if (engine.Init(1920, 1080)) //Ensures engine is initialised before running
	{
		engine.Run(); //Runs if engine is initialised
	}

	engine.Destroy(); //Destroys engine if init fails or engine.run finishes
	return 0;
};