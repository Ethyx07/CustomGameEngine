#include "scene/components/PlayerControllerComponent.h"
#include "input/InputManager.h"
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

namespace eng
{
	void PlayerControllerComponent::Update(float deltaTime)
	{
		if (!GetIsActive()) 
		{
			return;
		}
		auto& inputManager = Engine::GetInstance().GetInputManager();
		auto rotation = owner->GetRotation();

		if (inputManager.IsMousePositionChanged())
		{
			//Gets current pos and old pos to find the difference and sets the x y difference as delta X and delta Y. Used to determine how fast camera moves and its rotation
			const auto& oldPos = inputManager.GetOldMousePosition();
			const auto& currentPos = inputManager.GetCurrentMousePosition();

			float deltaX = currentPos.x - oldPos.x;
			float deltaY = currentPos.y - oldPos.y;


			//Rotation around y axis
			float yDeltaAngle = -deltaX * sensitivity * deltaTime;
			curYRot += yDeltaAngle;
			glm::quat yRot = glm::angleAxis(glm::radians(curYRot), glm::vec3(0.0f, 1.0f, 0.0f));

			//Rotation around x axis
			float xDeltaAngle = -deltaY * sensitivity * deltaTime;
			curXRot += xDeltaAngle;
			curXRot = std::clamp(curXRot, -89.0f, 89.0f);
			glm::quat xRot = glm::angleAxis(glm::radians(curXRot), glm::vec3(1.0f,0.0f,0.0f));

			rotation = glm::normalize(yRot * xRot);

			owner->SetRotation(rotation);
		}

		glm::vec3 forwardVector = rotation * glm::vec3(0.0f, 0.0f, -1.0f); //Gets the normalised direction of our forward vector
		glm::vec3 rightVector = rotation * glm::vec3(1.0f, 0.0f, 0.0f); //Gets the normalised direction of our right vector

		auto position = owner->GetPosition();
		if (inputManager.isKeyPressed(GLFW_KEY_SPACE))
		{
			moveSpeed = 10.0f;
		}
		else
		{
			moveSpeed = 2.0f;
		}
		//Left/Right Movement
		if (inputManager.isKeyPressed(GLFW_KEY_A))
		{
			position -= rightVector * moveSpeed * deltaTime;
		}
		if (inputManager.isKeyPressed(GLFW_KEY_D)) {
			position += rightVector * moveSpeed * deltaTime;
		}
		//Forward/Backward Movement
		if (inputManager.isKeyPressed(GLFW_KEY_W))
		{
			position += forwardVector * moveSpeed * deltaTime;
		}
		if (inputManager.isKeyPressed(GLFW_KEY_S)) {
			position -= forwardVector * moveSpeed * deltaTime;
		}


		owner->SetPosition(position);
	}

	void PlayerControllerComponent::SetIsActive(bool bActive)
	{
		bIsActive = bActive;
	}

	bool PlayerControllerComponent::GetIsActive()
	{
		return bIsActive;
	}

}