#pragma once
#include <string>
#include <vector>
#include <memory>

namespace eng
{
	class GameObject
	{
	public:
		virtual ~GameObject() = default;
		virtual void Update(float deltaTime);
		const std::string& GetName() const;
		void SetName(const std::string& newName);
		GameObject* GetParent();
		bool GetIsAlive() const;
		void MarkForDestroy();

	protected:
		GameObject() = default;

	private:
		std::string name;
		GameObject* parent = nullptr;
		std::vector<std::unique_ptr<GameObject>> children;
		bool bIsAlive = true;

		friend class Scene;
	};
}