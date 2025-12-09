#include "Engine.h"
#include "Application.h"

namespace engine
{
	bool Engine::Init()
	{
		if(!application) //Ensures application is valid before attempting to initialise
		{
			return false;
		}
		return application->Init();
	}

	void Engine::Run()
	{
		if (!application)
		{
			return;
		}

		lastTimePoint = std::chrono::high_resolution_clock::now(); //Gets last time point prior to application beginning
		while(!application->GetNeedsToBeClosed())
		{
			auto now = std::chrono::high_resolution_clock::now(); //Gets current time point
			float deltaTime = std::chrono::duration<float>(now - lastTimePoint).count(); //Finds difference between current and past to get our delta time
			lastTimePoint = now; //Updates last time point to be the current one

			application->Update(deltaTime); //Calls update function for application
		}
	}

	void Engine::Destroy()
	{
		if (application) //Destroys application and resets unique ptr
		{
			application->Destroy();
			application.reset();
		}
	}

	void Engine::SetApplication(Application* app)
	{
		application.reset(app);
	}

	Application* Engine::GetApplication()
	{
		return application.get();
	}

}