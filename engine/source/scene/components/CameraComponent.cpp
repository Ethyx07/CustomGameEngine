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
		return glm::inverse(owner->GetWorldTransform());
	}

	glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
	{
		return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
		return glm::mat4(1.0f);
	}
}