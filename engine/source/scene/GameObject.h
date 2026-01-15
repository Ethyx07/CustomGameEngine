#pragma once
#include "scene/Component.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace eng
{
	class Scene;

	enum class ObjectType
	{
		DEFAULT,
		DESTRUCTABLE
	};

	class GameObject
	{
	public:
		virtual ~GameObject() = default;
		virtual void Update(float deltaTime);
		virtual void LoadProperties(const nlohmann::json& json);
		virtual void Init();
		const std::string& GetName() const;
		void SetName(const std::string& newName);
		GameObject* GetParent();
		bool SetParent(GameObject* parent);
		GameObject* FindChildByName(const std::string& childName);
		Scene* GetScene();
		bool GetIsAlive() const;
		void MarkForDestroy();

		bool IsActive();
		void SetActive(bool active);

		void AddComponent(Component* component);
		template<typename T>
		T* GetComponentExact() //Gets the exact component. If wanting to return component & child classes use GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "GetComponentExact<T>: T must derive from Component class");

			size_t typeId = Component::StaticTypeId<T>();

			for (auto& component : components) //Loops through all components and tries to find the one that the component we are getting shares 
			{
				if (component->GetTypeId() == typeId)
				{
					return static_cast<T*>(component.get()); //Returns reference to that component
				}
			}
			return nullptr;
		}

		template<typename T>
		T* GetComponent() //Gets component of type T. Returned component can be child of type T
		{
			static_assert(std::is_base_of_v<Component, T>, "GetComponent<T>: T must derive from Component class");
			for (auto& component : components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get())) //Checks if component is of Type T or derived from it
				{
					return castedComponent;
				}
			}
			return nullptr;
		}

		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec3& pos);

		const glm::vec3 GetWorldPosition() const;
		void SetWorldPosition(const glm::vec3& pos);

		const glm::quat& GetRotation() const;
		void SetRotation(const glm::quat& rot);

		const glm::quat GetWorldRotation() const;
		void SetWorldRotation(const glm::quat& rot);

		const glm::vec3& GetScale() const;
		void SetScale(const glm::vec3& scal);

		const ObjectType GetObjectType() const;
		void SetObjectType(const ObjectType objT);

		glm::mat4 GetLocalTransform() const;
		glm::mat4 GetWorldTransform() const;

		glm::vec2 GetWorldPosition2D() const;
		glm::vec2 GetPosition2D() const;
		void SetPostion2D(const glm::vec2& pos);

		float GetRotation2D() const;
		void SetRotation2D(float rot);

		glm::vec2 GetScale2D() const;
		void SetScale2D(const glm::vec2 scal);

		glm::mat4 GetLocalTransform2D() const;
		glm::mat4 GetWorldTransform2D() const;


		static GameObject* LoadGLTF(const std::string& path, Scene* gameScene);


	protected:
		GameObject() = default;

	protected:
		std::string name;
		GameObject* parent = nullptr;
		Scene* scene = nullptr;
		std::vector<std::unique_ptr<GameObject>> children;
		std::vector<std::unique_ptr<Component>> components;
		bool bIsAlive = true;
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		bool bActive = true;

		ObjectType objType = ObjectType::DEFAULT;


		friend class Scene;
	};

	struct ObjectCreatorBase
	{
		virtual ~ObjectCreatorBase() = default;
		virtual GameObject* CreateGameObject() = 0;
	};

	template <typename T>
	struct ObjectCreator : ObjectCreatorBase
	{
		GameObject* CreateGameObject()
		{
			return new T();
		}
	};

	class GameObjectFactory
	{
	public:
		static GameObjectFactory& GetInstance();

		template <typename T>
		void RegisterObject(const std::string& name)
		{
			creators.emplace(name, std::make_unique<ObjectCreator<T>>());
		}

		GameObject* CreateGameObject(const std::string& typeName);

	private:
		std::unordered_map<std::string, std::unique_ptr<ObjectCreatorBase>> creators;
	};


#define GAMEOBJECT(ObjectClass) \
public: \
static void Register() {eng::GameObjectFactory::GetInstance().RegisterObject<ObjectClass>(std::string(#ObjectClass));}
}