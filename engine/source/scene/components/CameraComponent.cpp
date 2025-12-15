#include "scene/components/CameraComponent.h"
#include "scene/GameObject.h"

namespace eng
{
	void CameraComponent::Update(float deltaTime)
	{

	}

	glm::mat4 CameraComponent::GetViweMatrix() const //View matrix is the inverse of the world transform matrix
	{
		return glm::inverse(owner->GetWorldTransform());
	}

	glm::mat4 CameraComponent::GetProjectionMatrix() const
	{
		return glm::mat4(1.0f);
	}
}