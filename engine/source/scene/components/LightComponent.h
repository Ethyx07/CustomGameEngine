#pragma once
#include "scene/Component.h"
#include <glm/vec3.hpp>


namespace eng
{
	class LightComponent : public Component
	{
		COMPONENT(LightComponent)
	public:
		
		void LoadProperties(const nlohmann::json& json) override;
		void Update(float deltaTime) override;
		
		void SetColour(const glm::vec3& newColour);
		const glm::vec3& GetColour() const;
		
	private:
		glm::vec3 colour = glm::vec3(1.0f); //Defaults value to be white light (1.0f,1.0f,1.0f)
	};
}