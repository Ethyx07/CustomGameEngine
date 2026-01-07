#pragma once
#include "scene/Component.h"
#include <memory>

namespace eng
{
	class Material;
	class Mesh;

	class MeshComponent : public Component
	{
		COMPONENT(MeshComponent)
	public:
		MeshComponent() = default;
		MeshComponent(const std::shared_ptr<Material>& mat, const std::shared_ptr<Mesh>& compMesh);
		void Update(float deltaTime) override;

		void SetMaterial(const std::shared_ptr<Material>& mat);
		void SetMesh(const std::shared_ptr<Mesh>& newMesh);

	private:
		std::shared_ptr<Material> material;
		std::shared_ptr<Mesh> mesh;
	};
}