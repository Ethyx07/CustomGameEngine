#include "scene/components/MeshComponent.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include "render/RenderQueue.h"
#include "scene/GameObject.h"
#include "Engine.h"

namespace eng
{
	MeshComponent::MeshComponent(const std::shared_ptr<Material>& mat, const std::shared_ptr<Mesh>& compMesh) 
		: material(mat), mesh(compMesh)
	{

	}

	void MeshComponent::LoadProperties(const nlohmann::json& json)
	{
		//Material Loading
		if (json.contains("material"))
		{
			auto& matObj = json["material"];
			const std::string matPath = matObj.value("path", "");
			auto material = Material::Load(matPath);

			if (material) //Only sets material if it is valid
			{
				material->SetParam("colour", glm::vec3(1.0f)); //If uses the colour tint, defaulted as white
				if (matObj.contains("params")) //If there are parameters, it checks what type, looping through the complete list of them
				{
					auto& paramObj = matObj["params"];

					//One Parameter Floats
					if (paramObj.contains("float"))
					{
						for (auto param : paramObj["float"])
						{
							std::string name = param.value("name", "");
							float value = param.value("value", 0.0f);
							material->SetParam(name, value);
						}

					}

					//2 Float Params
					if (paramObj.contains("float2"))
					{
						for (auto param : paramObj["float2"])
						{
							std::string name = param.value("name", "");
							float value0 = param.value("value0", 0.0f);
							float value1 = param.value("value1", 0.0f);
							material->SetParam(name, value0, value1);
						}

					}

					//3 Float Params
					if (paramObj.contains("float3"))
					{
						for (auto param : paramObj["float3"])
						{
							std::string name = param.value("name", "");
							float value0 = param.value("value0", 0.0f);
							float value1 = param.value("value1", 0.0f);
							float value2 = param.value("value2", 0.0f);
							material->SetParam(name, glm::vec3(value0, value1, value2));
						}
					}

					//Textures
					if (paramObj.contains("textures"))
					{
						for (auto& param : paramObj["textures"])
						{
							std::string name = param.value("name", "");
							std::string texPath = param.value("path", "");
							auto texture = Texture::Load(texPath);

							material->SetParam(name, texture);
						}
					}
				}
			}
			SetMaterial(material);
		}


		//Mesh Loading
		if (json.contains("mesh"))
		{
			const auto& meshData = json["mesh"];
			const std::string type = meshData.value("type", "");
			if (type == "box") //Currently generation works for basic box meshes. GLTF uses other method
			{
				glm::vec3 extents(
					meshData.value("x", 1.0f),
					meshData.value("y", 1.0f),
					meshData.value("z", 1.0f)
				);

				auto mesh = Mesh::CreateBox(extents);
				SetMesh(mesh);
			}
			else if (type == "sphere")
			{
				const float radius = meshData.value("radius", 1.0f);
				auto mesh = Mesh::CreateSphere(radius, 32, 32);

				SetMesh(mesh);
			}
		}
	}

	void MeshComponent::Update(float deltaTime)
	{
		if (!material || !mesh)
		{
			return;
		}

		RenderCommand command;
		command.material = material.get();
		command.mesh = mesh.get();
		command.modelMatrix = GetOwner()->GetWorldTransform();

		auto& renderQueue = Engine::GetInstance().GetRenderQueue();
		renderQueue.Submit(command);
	}

	void MeshComponent::SetMaterial(const std::shared_ptr<Material>& mat)
	{
		material = mat;
	}

	void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& newMesh)
	{
		mesh = newMesh;
	}
}