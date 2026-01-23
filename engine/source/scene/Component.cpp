#include "scene/Component.h"

namespace eng
{
	size_t Component::nextId = 1;

	void Component::LoadProperties(const nlohmann::json& json)
	{
	}

	void Component::Init()
	{
	}

	void Component::Update(float deltaTime)
	{

	}

	GameObject* Component::GetOwner()
	{
		return owner;
	}

	ComponentFactory& ComponentFactory::GetInstance()
	{
		static ComponentFactory instance;
		return instance;
	}

	Component* ComponentFactory::CreateComponent(const std::string& typeName)
	{
		auto iterator = creators.find(typeName);
		if (iterator == creators.end())
		{
			return nullptr;
		}
		return iterator->second->CreateComponent();
	}
}