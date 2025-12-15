#pragma once
#include "scene/Component.h"
#include <memory>

namespace eng
{
	class Material;
	class Mesh;

	class MeshComponent : public Component
	{
	public:
		MeshComponent(const std::shared_ptr<Material>& mat, const std::shared_ptr<Mesh>& compMesh);
		void Update(float deltaTime) override;

		void SetMaterial(const std::shared_ptr<Material>& mat);

	private:
		std::shared_ptr<Material> material;
		std::shared_ptr<Mesh> mesh;
	};
}