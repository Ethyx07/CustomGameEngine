#pragma once
#include "Common.h"
#include "scene/Component.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <memory>

namespace eng
{
	class UIElementComponent;
	class Texture;
	class Mesh;

	class CanvasComponent : public Component
	{
		COMPONENT(CanvasComponent)

	public:
		void Init() override;
		void Update(float deltaTime) override;
		void UpdateBatches(Texture* texture);
		void Render(UIElementComponent* element);
		void BeginRendering();
		void Flush();
		void CollectUI(UIElementComponent* element, std::vector<UIElementComponent*>& outVec);

		void DrawRect(const glm::vec2& p1, const glm::vec2& p2,
			const glm::vec2& uv1, const glm::vec2& uv2,
			Texture* texture, const glm::vec4& colour);

	private:
		std::vector<UIBatch> batches;
		std::vector<float> vertices; //pos(2), colour(4), uv(2)
		std::vector<uint32_t> indices;
		std::shared_ptr<Mesh> mesh;
	};
}