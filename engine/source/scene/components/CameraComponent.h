#pragma once
#include "scene/Component.h"
#include <glm/mat4x4.hpp>

namespace eng
{
	class CameraComponent : public Component
	{
		COMPONENT(CameraComponent)

	public:
		void Update(float deltaTime) override;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix(float aspect) const;

	private: 
		float fov = 60.0f; //These values create the triangle/pyramid of the cameras render area. This is the angle it spreads
		float nearPlane = 0.1f; //The closest plane it renders
		float farPlane = 1000.0f; //The furthest plane it renders
	};
}