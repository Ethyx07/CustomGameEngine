#include "scene/components/LightComponent.h"

namespace eng
{
	void LightComponent::LoadProperties(const nlohmann::json& json)
	{
		if (json.contains("colour"))
		{
			auto colourData = json["colour"];
			glm::vec3 colour(colourData.value("r", 1.0f),
				colourData.value("g", 1.0f),
				colourData.value("b", 1.0f));

			SetColour(colour);
		}
	}

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