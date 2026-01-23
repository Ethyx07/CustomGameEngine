#pragma once
#include "input/InputManager.h"
#include "graphics/GraphicsAPI.h"
#include "render/RenderQueue.h"
#include "graphics/Texture.h"
#include "scene/Scene.h"
#include "io/FileSystem.h"
#include "physics/PhysicsManager.h"
#include "audio/AudioManager.h"
#include "font/FontManager.h"
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
		TextureManager& GetTextureManager();
		PhysicsManager& GetPhysicsManager();
		AudioManager& GetAudioManager();
		FontManager& GetFontManager();

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
		TextureManager textureManager;
		PhysicsManager physicsManager;
		AudioManager audioManager;
		FontManager fontManager;

		std::unique_ptr<Scene> currentScene;
		
	};
}

