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
		auto& inputManager = Engine::GetInstance().GetInputManager();
		auto rotation = owner->GetRotation();

		if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			//Gets current pos and old pos to find the difference and sets the x y difference as delta X and delta Y. Used to determine how fast camera moves and its rotation
			const auto& oldPos = inputManager.GetOldMousePosition();
			const auto& currentPos = inputManager.GetCurrentMousePosition();

			float deltaX = currentPos.x - oldPos.x;
			float deltaY = currentPos.y - oldPos.y;

			//Rotation around y axis
			rotation.y -= deltaX * sensitivity * deltaTime;

			//Rotation around x axis
			rotation.x -= deltaY * sensitivity * deltaTime;

			owner->SetRotation(rotation);
		}

		glm::mat4 rotationMatrix(1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotation on x axis
		rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); //Rotation on y axis
		rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); //Rotation on z axis

		glm::vec3 forwardVector = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f))); //Gets the normalised direction of our forward vector
		glm::vec3 rightVector = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f))); //Gets the normalised direction of our right vector

		auto position = owner->GetPosition();

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
}