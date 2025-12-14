#include "scene/Scene.h"

namespace eng
{
	void Scene::Update(float deltaTime)
	{
		for (auto iterator = objects.begin(); it != objects.end()) //Loops through vector of gameobject children
		{
			if ((*iterator)->GetIsAlive()) //Updates it if its alive
			{
				(*iterator)->Update(deltaTime);
				++iterator;
			}
			else
			{
				(*iterator) = objects.erase(iterator) //If its not alive/getting destroyed it is erased from the vector
			}
		}
	}

	void  Scene::Clear()
	{
		objects.clear()
	}

	GameObject* Scene::CreateObject(const std::string& name, GameObject* parent = nullptr)
	{
		auto obj = new GameObject();
		obj->SetName(name);
		SetParent(obj, parent);
		return obj;
	}

	void Scene::SetParent(GameObject* obj, GameObject* parent)
	{

	}
}