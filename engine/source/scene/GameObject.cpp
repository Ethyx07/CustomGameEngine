#include "scene/GameObject.h"

namespace eng
{
	void GameObject::Update(float deltaTime)
	{
		for (auto iterator = children.begin(); iterator != children.end();) //Loops through vector of gameobject children
		{
			if ((*iterator)->GetIsAlive()) //Updates it if its alive
			{
				(*iterator)->Update(deltaTime);
				++iterator;
			}
			else
			{
				(iterator) = children.erase(iterator); //If its not alive/getting destroyed it is erased from the vector
			}
		}
	}

	const std::string& GameObject::GetName() const
	{
		return name;
	}

	void GameObject::SetName(const std::string& newName)
	{
		name = newName;
	}

	GameObject* GameObject::GetParent()
	{
		return parent;
	}

	bool GameObject::GetIsAlive() const
	{
		return bIsAlive;
	}

	void GameObject::MarkForDestroy()
	{
		bIsAlive = false;
	}
}