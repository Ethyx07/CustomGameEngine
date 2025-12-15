#pragma once
#include "scene/Component.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

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

		void AddComponent(Component* component);
		template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<Component, T>>
		T* GetComponent()
		{
			size_t typeId = Component::StaticType<T>();

			for (auto& component : components)
			{
				if (component->GetTypeId() == typeId)
				{
					return static_cast<T*>(component.get());
				}
			}
			return nullptr;
		}

		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec3& pos);

		const glm::vec3& GetRotation() const;
		void SetRotation(const glm::vec3& rot);

		const glm::vec3& GetScale() const;
		void SetScale(const glm::vec3& scal);

		glm::mat4 GetLocalTransform() const;
		glm::mat4 GetWorldTransform() const;


	protected:
		GameObject() = default;

	private:
		std::string name;
		GameObject* parent = nullptr;
		std::vector<std::unique_ptr<GameObject>> children;
		std::vector<std::unique_ptr<Component>> components;
		bool bIsAlive = true;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);


		friend class Scene;
	};
}