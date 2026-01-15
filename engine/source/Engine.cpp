#include "Engine.h"
#include "Application.h"
#include "scene/GameObject.h"
#include "scene/Component.h"
#include "scene/components/CameraComponent.h"
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

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int)
	{
		auto& inputManager = eng::Engine::GetInstance().GetInputManager();
		if (action == GLFW_PRESS) //Checks if button is pressed. Game can then use this in return
		{
			inputManager.SetMouseButtonPressed(button, true);
		}
		else if (action == GLFW_RELEASE)
		{
			inputManager.SetMouseButtonPressed(button, false);
		}
	}

	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		auto& inputManager = eng::Engine::GetInstance().GetInputManager();
		inputManager.SetOldMousePosition(inputManager.GetCurrentMousePosition()); //Sets current pos to old pos

		glm::vec2 currentPos(static_cast<float>(xpos), static_cast<float>(ypos));
		inputManager.SetCurrentMousePosition(currentPos);
		inputManager.SetMousePositionChanged(true);
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

		Scene::RegisterTypes();
		application->RegisterTypes();

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
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, cursorPositionCallback); //Sets the callbacks for the mouse input and position for look action

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Disables OS cursor and locks cursor to window
		glfwMakeContextCurrent(window); //Sets window as glfw current context

		if (glewInit() != GLEW_OK)  //Checks if glew was initialised properly
		{
			glfwTerminate();
			return false;
		}
		graphicsAPI.Init();
		renderQueue.Init();
		physicsManager.Init();
		audioManager.Init();
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

			physicsManager.Update(deltaTime); //Updates physics within the engine loop
			application->Update(deltaTime); //Calls update function for the application

			graphicsAPI.SetClearColour(1.0f, 1.0f, 1.0f, 1.0f);
			graphicsAPI.ClearBuffers();

			CameraData cameraData;
			std::vector<LightData> lights;
			int width = 0;
			int height = 0;

			glfwGetWindowSize(window, &width, &height);
			float aspect = static_cast<float>(width) / static_cast<float>(height); //Gets the aspect ratio of the screen based on the current window

			if (currentScene)
			{
				if (auto cameraObject = currentScene->GetMainCamera()) 
				{
					auto cameraComponent = cameraObject->GetComponent<CameraComponent>(); //Gets the current cameras cameraData to pass to our graphicsAPI
					if (cameraComponent)
					{
						cameraData.viewMatrix = cameraComponent->GetViewMatrix();
						cameraData.projectionMatrix = cameraComponent->GetProjectionMatrix(aspect);
						cameraData.position = cameraObject->GetWorldPosition();
						cameraData.orthographicMatrix = glm::ortho(
							0.0f, static_cast<float>(width),
							0.0f, static_cast<float>(height));
					}
				}

				lights = currentScene->CollectLights();
			}

			renderQueue.Draw(graphicsAPI, cameraData, lights);

			glfwSwapBuffers(window);

			inputManager.SetOldMousePosition(inputManager.GetCurrentMousePosition());
			inputManager.SetMousePositionChanged(false); //Resets value to false each frame
		}
	}

	void Engine::Destroy()
	{
		if (application) //Destroys application and resets unique ptr
		{
			application->Destroy();
			application.reset();
		}
		glfwDestroyWindow(window);
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

	RenderQueue& Engine::GetRenderQueue()
	{
		return renderQueue;
	}

	FileSystem& Engine::GetFileSystem()
	{
		return fileSystem;
	}

	TextureManager& Engine::GetTextureManager()
	{
		return textureManager;
	}

	PhysicsManager& Engine::GetPhysicsManager()
	{
		return physicsManager;
	}

	AudioManager& Engine::GetAudioManager()
	{
		return audioManager;
	}

	void Engine::SetScene(Scene* scene)
	{
		currentScene.reset(scene);
	}

	Scene* Engine::GetScene()
	{
		return currentScene.get();
	}

	
}