#pragma once
#include "input/InputManager.h"
#include "graphics/GraphicsAPI.h"
#include "render/RenderQueue.h"
#include "scene/Scene.h"
#include "io/FileSystem.h"
#include <memory>
#include <chrono>

struct GLFWwindow;

namespace eng
{
	class Application;
	class Engine
	{
	public:
		static Engine& GetInstance();
	
	private: //Creates default constructor and prevents any copying or moving of the engine
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator = (const Engine&) = delete;
		Engine& operator = (Engine&&) = delete;

	public:
		bool Init(int width, int height);
		void Run();
		void Destroy();

		void SetApplication(Application* app);
		Application* GetApplication();
		InputManager& GetInputManager();
		GraphicsAPI& GetGraphicsAPI();
		RenderQueue& GetRenderQueue();
		FileSystem& GetFileSystem();

		void SetScene(Scene* scene);
		Scene* GetScene();

	private:
		std::unique_ptr<Application> application; 
		std::chrono::steady_clock::time_point lastTimePoint;
		GLFWwindow* window = nullptr;
		InputManager inputManager;
		GraphicsAPI graphicsAPI;
		RenderQueue renderQueue;
		FileSystem fileSystem;
		std::unique_ptr<Scene> currentScene;
		
	};
}

