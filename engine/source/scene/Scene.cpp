#include "scene/Scene.h"

namespace eng
{
	void Scene::Update(float deltaTime)
	{
		for (auto iterator = objects.begin(); iterator != objects.end();) //Loops through vector of gameobject children
		{
			if ((*iterator)->GetIsAlive()) //Updates it if its alive
			{
				(*iterator)->Update(deltaTime);
				++iterator;
			}
			else
			{
				(iterator) = objects.erase(iterator); //If its not alive/getting destroyed it is erased from the vector
			}
		}
	}

	void  Scene::Clear()
	{
		objects.clear();
	}

	GameObject* Scene::CreateObject(const std::string& name, GameObject* parent)
	{
		auto obj = new GameObject();
		obj->SetName(name);
		SetParent(obj, parent);
		return obj;
	}

	bool Scene::SetParent(GameObject* obj, GameObject* parent)
	{
		bool result = false;
		auto currentParent = obj->GetParent();

		if (parent == nullptr) //Moving object to scene root
		{
			if (currentParent != nullptr) //Object has a parent
			{
				auto it = std::find_if( //Loops through entire vector of children for the current parent, calling the lambda function on each until element == obj
					currentParent->children.begin(),
					currentParent->children.end(),
					[obj](const std::unique_ptr<GameObject>& element)
					{
						return element.get() == obj;
					});

				if (it != currentParent->children.end()) //If its equal to end then it isnt in the list
				{
					objects.push_back(std::move(*it)); //Moves ownership to objects
					obj->parent = nullptr;
					currentParent->children.erase(it); //Removes itself from children list
					result = true;
				}
			}
			else //No current parent. We have 2 cases of what this can be. Obj is in scene root or object is just created
			{
				auto it = std::find_if( //Loops through entire vector of root scene to see if it is at the root
					objects.begin(),
					objects.end(),
					[obj](const std::unique_ptr<GameObject>& element)
					{
						return element.get() == obj;
					});
				if (it == objects.end()) //Means that the object has just been created.
				{
					std::unique_ptr<GameObject> objHolder(obj);
					objects.push_back(std::move(objHolder));
					result = true;
				} //No need to do the other case as other case is that the obj is at the scene root and parent = nullptr would move it to the scene root (so it wouldnt move)
			}
		}
		
		else //Trying to add it as a child of another object
		{
			if (currentParent != nullptr)//Case where obj has a current parent
			{
				auto it = std::find_if( //Loops through entire vector of children for the current parent, calling the lambda function on each until element == obj
					currentParent->children.begin(),
					currentParent->children.end(),
					[obj](const std::unique_ptr<GameObject>& element)
					{
						return element.get() == obj;
					});
				if (it != currentParent->children.end()) //Means obj is in the currentParent list of children
				{
					bool foundDescendent = false;
					auto currentElement = parent;
					while (currentElement) //Starts from new parent and works its way backwards until currentElement is null. Makes sure obj isnt a parent/grandparent etc of its new parent
					{
						if (currentElement == obj)
						{
							foundDescendent = true;
							break;
						}
						currentElement = currentElement->GetParent();
					}

					if (!foundDescendent)
					{
						parent->children.push_back(std::move(*it));
						obj->parent = parent;
						currentParent->children.erase(it);
						result = true;
					}
				}
			}
			//2 Cases. Current parent is null, either new object or at scene root
			else
			{
				auto it = std::find_if( //Loops through entire vector of root scene to see if it is at the root
					objects.begin(),
					objects.end(),
					[obj](const std::unique_ptr<GameObject>& element)
					{
						return element.get() == obj;
					});
				if (it == objects.end()) //Means that the object has just been created
				{
					std::unique_ptr<GameObject> objHolder(obj);
					objects.push_back(std::move(objHolder));
					result = true;
				}
				else
				{
					bool foundDescendent = false;
					auto currentElement = parent;
					while (currentElement) //Starts from new parent and works its way backwards until currentElement is null. Makes sure obj isnt a parent/grandparent etc of its new parent
					{
						if (currentElement == obj)
						{
							foundDescendent = true;
							break;
						}
						currentElement = currentElement->GetParent();
					}

					if (!foundDescendent) //Means object can safely be added without causing a circular dependency issue
					{
						parent->children.push_back(std::move(*it));
						obj->parent = parent;
						currentParent->children.erase(it);
						result = true;
					}
				}
			}
		}
	
		return result;
	}
}