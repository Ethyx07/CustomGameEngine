#pragma once

#include "nlohmann/json.hpp"
#include <cstddef>
#include <string>
#include <memory>

namespace eng
{

	class GameObject;
	class Component
	{
	public:
		virtual ~Component() = default;
		virtual void LoadProperties(const nlohmann::json& json);
		virtual void Init();
		virtual void Update(float deltaTime) = 0;
		virtual size_t GetTypeId() const = 0;
		
		GameObject* GetOwner();

		template<typename T>
		static size_t StaticTypeId() //Gives each class its own id. (Diff component classes get diff ids)
		{
			static size_t typeId = nextId++;
			return typeId;
		}

	protected:
		GameObject* owner = nullptr;

		friend class GameObject;

	private:
		static size_t nextId;
	};

	struct ComponentCreatorBase
	{
	public:
		virtual ~ComponentCreatorBase() = default;
		virtual Component* CreateComponent() = 0;
	};

	template <typename T>
	struct ComponentCreator : ComponentCreatorBase
	{
		Component* CreateComponent() override
		{
			return new T();
		}
	};

	class ComponentFactory //Component Factory singleton
	{
	public:
		static ComponentFactory& GetInstance();

		template <typename T>
		void RegisterComponent(const std::string& name) //Registers the creator
		{
			creators.emplace(name, std::make_unique<ComponentCreator<T>>());
		}

		Component* CreateComponent(const std::string& typeName); //Gets the creator and creates that type of component
	
	private:
		std::unordered_map<std::string, std::unique_ptr<ComponentCreatorBase>> creators;
	};



#define COMPONENT(ComponentClass) \
public: \
	static size_t TypeId() { return Component::StaticTypeId<ComponentClass>(); } \
	size_t GetTypeId() const override {return TypeId();} \
	static void Register() {eng::ComponentFactory::GetInstance().RegisterComponent<ComponentClass>(std::string(#ComponentClass));}

}