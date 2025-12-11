#include "Engine.h"
#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


namespace eng
{
	void keyCallback(GLFWwindow* window, int key, int, int action, int)
	{
		auto& inputManager = eng::Engine::GetInstance().GetInputManager();
		if (action == GLFW_PRESS) //Checks if key is pressed. Game can then use this in return
		{
			inputManager.SetKeyPressed(key, true);
		}
		else if (action == GLFW_RELEASE)
		{
			inputManager.SetKeyPressed(key, false);
		}
	}

	Engine& Engine::GetInstance() //Globally accessible engine
	{
		static Engine instance;
		return instance;
	}

	bool Engine::Init(int width, int height)
	{
		if(!application) //Ensures application is valid before attempting to initialise
		{
			return false;
		}

		if (!glfwInit()) //Makes sure glfw initialised properly
		{
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //tells glfw what version of opengl is being used and sets its profile
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, "GameDevelopmentProject", nullptr, nullptr); //Creates window terminal
		if (window == nullptr)
		{
			std::cout << "ERROR CREATING WINDOW" << std::endl;
			glfwTerminate(); //Terminates if window is not created
			return false;
		}

		glfwSetKeyCallback(window, keyCallback);

		glfwMakeContextCurrent(window); //Sets window as glfw current context

		if (glewInit() != GLEW_OK)  //Checks if glew was initialised properly
		{
			glfwTerminate();
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
		while(!glfwWindowShouldClose(window) && !application->GetNeedsToBeClosed())
		{
			glfwPollEvents();

			auto now = std::chrono::high_resolution_clock::now(); //Gets current time point
			float deltaTime = std::chrono::duration<float>(now - lastTimePoint).count(); //Finds difference between current and past to get our delta time
			lastTimePoint = now; //Updates last time point to be the current one

			application->Update(deltaTime); //Calls update function for the application

			glfwSwapBuffers(window);
		}
	}

	void Engine::Destroy()
	{
		if (application) //Destroys application and resets unique ptr
		{
			application->Destroy();
			application.reset();
		}
		glfwTerminate();
		window = nullptr;
	}

	void Engine::SetApplication(Application* app)
	{
		application.reset(app);
	}

	Application* Engine::GetApplication()
	{
		return application.get();
	}

	InputManager& Engine::GetInputManager()
	{
		return inputManager;
	}

	GraphicsAPI& Engine::GetGraphicsAPI()
	{
		return graphicsAPI;
	}

}