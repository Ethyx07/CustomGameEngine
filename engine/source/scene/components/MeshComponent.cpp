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