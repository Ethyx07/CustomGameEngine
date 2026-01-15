#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace eng
{
	struct CameraData
	{
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::vec3 position;
		glm::mat4 orthographicMatrix;
	};

	struct LightData
	{
		glm::vec3 colour;
		glm::vec3 position;
	};
}