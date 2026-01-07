#include "scene/Scene.h"
#include "scene/components/AnimationComponent.h"
#include "scene/components/CameraComponent.h"
#include "scene/components/LightComponent.h"
#include "scene/components/MeshComponent.h"
#include "scene/components/PhysicsComponent.h"
#include "scene/components/PlayerControllerComponent.h"
#include "Engine.h"


namespace eng
{
	void Scene::RegisterTypes() //Registers all engine component types
	{
		AnimationComponent::Register();
		CameraComponent::Register();
		LightComponent::Register();
		MeshComponent::Register();
		PhysicsComponent::Register();
		PlayerControllerComponent::Register();
	}

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
		obj->scene = this;
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
					parent->children.push_back(std::move(objHolder));
					obj->parent = parent; 
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
						objects.erase(it);
						result = true;
					}
				}
			}
		}
	
		return result;
	}

	void Scene::SetMainCamera(GameObject* camera) //Sets main camera
	{
		mainCamera = camera;
	}

	GameObject* Scene::GetMainCamera() //Returns main camera
	{
		return mainCamera;
	}

	std::vector<LightData> Scene::CollectLights()
	{
		std::vector<LightData> lights;
		for (auto& obj : objects) //Loops through all objects in the scene and collects the lights
		{
			CollectLightsRecursive(obj.get(), lights);
		}

		return lights;
	}

	void Scene::CollectLightsRecursive(GameObject* obj, std::vector<LightData>& out)
	{
		if (auto* light = obj->GetComponent<LightComponent>()) //If obj is valid. Once the obj is nullptr that parent to child group is completed
		{
			LightData data;
			data.colour = light->GetColour();
			data.position = obj->GetWorldPosition();
			out.push_back(data);
		}

		for (auto& child : obj->children) //Loops through the children and calls the same function
		{
			CollectLightsRecursive(child.get(), out);
		}
	}


	std::shared_ptr<Scene> Scene::Load(const std::string& path)
	{
		const std::string contents = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path); //Gets asset file as text
		if (contents.empty()) //Ensures something is loaded
		{
			return nullptr;
		}
		nlohmann::json j = nlohmann::json::parse(contents); //Parses contents to json
		if (j.empty())
		{
			return nullptr;
		}

		auto result = std::make_shared<Scene>(); //Creates scene shared

		const std::string sceneName = j.value("name", "NoName"); //Gets the name of the scene from the scene file, sets to noname if none is found

		if (j.contains("objects") && j["objects"].is_array()) //Only runs if json has an objects list and it is in an array
		{
			const auto& objects = j["objects"];
			for (const auto& obj : objects)
			{
				result->LoadObject(obj, nullptr); //As these are top-level objects they have no parent
			}
		}

		return result;
	}

	void Scene::LoadObject(const nlohmann::json& objectJSON, GameObject* parent)
	{
		const std::string name = objectJSON.value("name", "Object"); //Object json data is checked for object name

		GameObject* gameObject = nullptr;

		if (objectJSON.contains("type"))
		{
			std::string type = objectJSON.value("type", "");
			if (type == "gltf") //GLTF objects are here
			{
				//GLTF BRANCH
			}
			else //User defined objects are loaded here
			{

			}
		}
		else
		{
			gameObject = CreateObject(name, parent); //Creates default gameobject
		}

		if (!gameObject)
		{
			return; //GameObject unable to be created, so it bails out early
		}

		//Position Loading
		if (objectJSON.contains("position") && objectJSON["position"].is_array()) //Checks if json has position value and array
		{
			const auto& posData = objectJSON["position"]; 
			glm::vec3 pos(posData.value("x", 0.0f), //Gets posData values and creates a vec3 pos of it
				posData.value("y", 0.0f),
				posData.value("z", 0.0f)
			);
			gameObject->SetPosition(pos);
		}

		//Rotation Loading
		if (objectJSON.contains("rotation") && objectJSON["rotation"].is_array())
		{
			const auto& rotData = objectJSON["rotation"]; //Gets rotation data and converts it to glm::quat
			glm::quat rot(rotData.value("w", 0.0f), //Begins with rotation axis as this is how glm::quat organises it
				rotData.value("x", 0.0f),
				rotData.value("y", 0.0f),
				rotData.value("z", 0.0f)
			);
			gameObject->SetRotation(rot);
		}

		//Scale Loading
		if (objectJSON.contains("scale") && objectJSON["scale"].is_array())
		{
			const auto& scaleData = objectJSON["scale"]; //Gets scale data and converts it to glm::vec3
			glm::vec3 scale(scaleData.value("x", 0.0f),
				scaleData.value("y", 0.0f),
				scaleData.value("z", 0.0f)
			);
			gameObject->SetScale(scale);
		}

		gameObject->LoadProperties(objectJSON); //Loads properties for custom gameobjects

		if (objectJSON.contains("components") && objectJSON["components"].is_array())
		{
			const auto& compData = objectJSON["components"];

			for (const auto& componentJSON : compData)
			{
				const std::string compType = componentJSON.value("type", ""); //Gets the component type based on its value
				Component* component = ComponentFactory::GetInstance().CreateComponent(compType);
				if (component) //Ensures comptype is valid as otherwise component would be nullptr
				{
					component->LoadProperties(componentJSON);
					gameObject->AddComponent(component);
				}
			}
		}

		if (objectJSON.contains("children") && objectJSON["children"].is_array())
		{
			for (const auto& childJSON : objectJSON["children"])
			{
				LoadObject(childJSON, gameObject); //recursive
			}
		}

		gameObject->Init(); //Finally calls gameobjects initialisation function
	}

}