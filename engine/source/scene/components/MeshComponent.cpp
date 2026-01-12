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

			if (material)
			{
				SetMaterial(material);
			}
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