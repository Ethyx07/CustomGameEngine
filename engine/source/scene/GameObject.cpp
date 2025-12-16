#include "scene/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>


namespace eng
{
	void GameObject::Update(float deltaTime)
	{
		for (auto& component : components)
		{
			component->Update(deltaTime);
		}
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

	void GameObject::AddComponent(Component* component)
	{
		components.emplace_back(component);
		component->owner = this;
	}

	const glm::vec3& GameObject::GetPosition() const
	{
		return position;
	}

	void GameObject::SetPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	const glm::quat& GameObject::GetRotation() const
	{
		return rotation;
	}

	void GameObject::SetRotation(const glm::quat& rot)
	{
		rotation = rot;
	}

	const glm::vec3& GameObject::GetScale() const
	{
		return scale;
	}

	void GameObject::SetScale(const glm::vec3& scal)
	{
		scale = scal;
	}

	glm::mat4 GameObject::GetLocalTransform() const
	{
		glm::mat4 matrix = glm::mat4(1.0f); //Creates identity matrix

		//Translation
		matrix = glm::translate(matrix, position);

		//Rotation
		matrix = matrix * glm::mat4_cast(rotation); //Converts quaternion into rotation matrix
		
		//Scale
		matrix = glm::scale(matrix, scale);

		return matrix;
	}

	glm::mat4 GameObject::GetWorldTransform() const
	{
		if (parent)
		{
			return parent->GetWorldTransform() * GetLocalTransform(); //Keeps calling world transforms for parents until it reaches the top parent
		}
		else return GetLocalTransform();
	}
}