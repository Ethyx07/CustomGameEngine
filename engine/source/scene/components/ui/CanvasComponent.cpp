#include "scene/components/ui/CanvasComponent.h"
#include "scene/components/ui/UIElementComponent.h"

#include "scene/GameObject.h"
#include "graphics/VertexLayout.h"
#include "render/Mesh.h"
#include "Engine.h"

namespace eng
{
	void CanvasComponent::Init()
	{
		VertexLayout layout;
		//Position
		layout.elements.push_back({ VertexElement::PositionIndex, 2, GL_FLOAT, 0 });
		//Colour
		layout.elements.push_back({ VertexElement::ColourIndex, 4, GL_FLOAT, sizeof(float) * 2 });
		//UV
		layout.elements.push_back({ VertexElement::UVIndex, 2, GL_FLOAT, sizeof(float) * 6 });
		layout.stride = sizeof(float) * 8;

		mesh = std::make_shared<Mesh>(layout, vertices, indices);
	}

	void CanvasComponent::Update(float deltaTime)
	{
		BeginRendering();
		const auto& children = owner->GetChildren(); //Gets array of children pointers
		for (const auto& child : children)
		{
			if (auto component = child->GetComponent<UIElementComponent>()) //Gets component if child object has it
			{
				Render(component); //Renders each component that is of type UIElementComponent
			}
		}
		Flush();
	}

	void CanvasComponent::Render(UIElementComponent* element)
	{
		if (!element)
		{
			return;
		}
		//Render this element
		element->Render(this); //Calls the elements render function, passing in this canvas

		//Recursively renders all children that are also UI elements
		const auto& children = element->GetOwner()->GetChildren();
		for (const auto& child : children)
		{
			if (auto component = child->GetComponent<UIElementComponent>())
			{
				Render(component);
			}
		}
	}

	void CanvasComponent::DrawRect(const glm::vec2& p1, const glm::vec2& p2,
		const glm::vec2& uv1, const glm::vec2& uv2,
		Texture* texture, const glm::vec4& colour)
	{
		uint32_t base = static_cast<uint32_t>(vertices.size() / 8);

		//p1 = lower left, p2 = upper right. Same goes for uv1 and uv2
		vertices.insert(vertices.end(), {
			p2.x, p2.y, colour.r, colour.g, colour.b, colour.a, uv2.x, uv2.y, //Upper Right
			p1.x, p2.y, colour.r, colour.g, colour.b, colour.a, uv1.x, uv2.y, //Upper Left
			p1.x, p1.y, colour.r, colour.g, colour.b, colour.a, uv1.x, uv1.y, //Lower Left
			p2.x, p1.y, colour.r, colour.g, colour.b, colour.a, uv2.x, uv1.y //Lower Right
			});

		indices.insert(indices.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });

		UpdateBatches(texture);
	}

	void CanvasComponent::UpdateBatches(Texture* texture)
	{
		if (batches.empty() || batches.back().texture != texture)
		{
			batches.push_back({ texture, 6 });
		}
		else
		{
			batches.back().indexCount += 6;
		}
	}

	void CanvasComponent::BeginRendering()
	{
		vertices.clear();
		indices.clear();
		batches.clear();
	}

	void CanvasComponent::Flush()
	{
		mesh->UpdateDynamic(vertices, indices);
		auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
		const auto& viewport = graphicsAPI.GetViewport();
		RenderCommandUI command;
		command.mesh = mesh.get();
		command.shaderProgram = graphicsAPI.GetDefaultUIShaderProgram().get();
		command.batches = batches;
		command.screenWidth = viewport.width;
		command.screenHeight = viewport.height;
		Engine::GetInstance().GetRenderQueue().Submit(command);
	}

	void CanvasComponent::CollectUI(UIElementComponent* element, std::vector<UIElementComponent*>& outVec)
	{

	}
}