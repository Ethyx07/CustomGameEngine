#pragma once
#include <memory>
#include <chrono>

struct GLFWwindow;

namespace eng
{
	class Application;
	class Engine
	{
	public:
		bool Init(int width, int height);
		void Run();
		void Destroy();

		void SetApplication(Application* app);
		Application* GetApplication();

	private:
		std::unique_ptr<Application> application; 
		std::chrono::steady_clock::time_point lastTimePoint;
		GLFWwindow* window = nullptr;
	};
}

