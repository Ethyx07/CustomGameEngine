#include "scene/components/LightComponent.h"

namespace eng
{
	void LightComponent::Update(float deltaTime)
	{

	}

	void LightComponent::SetColour(const glm::vec3& newColour)
	{
		colour = newColour;
	}

	const glm::vec3& LightComponent::GetColour() const
	{
		return colour;
	}
}