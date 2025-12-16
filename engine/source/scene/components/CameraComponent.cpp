#include "scene/components/CameraComponent.h"
#include "scene/GameObject.h"
#include "glm/gtc/matrix_transform.hpp"

namespace eng
{
	void CameraComponent::Update(float deltaTime)
	{

	}

	glm::mat4 CameraComponent::GetViewMatrix() const //View matrix is the inverse of the world transform matrix
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::mat4_cast(owner->GetRotation());
		matrix[3] = glm::vec4(owner->GetPosition(), 1.0f);

		if (owner->GetParent())
		{
			matrix = owner->GetParent()->GetWorldTransform() * matrix;
		}

		return glm::inverse(matrix);
	}

	glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
	{
		return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
		return glm::mat4(1.0f);
	}
}