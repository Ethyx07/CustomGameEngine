#include "scene/components/PlayerControllerComponent.h"
#include "input/InputManager.h"
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

namespace eng
{

	void PlayerControllerComponent::Init()
	{
		kinematicController = std::make_unique<KinematicCharacterController>(0.4f, 1.2f);
	}

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

		glm::vec3 move(0.0f);
		
		//Left/Right Movement
		if (inputManager.isKeyPressed(GLFW_KEY_A))
		{
			move -= rightVector;
		}
		if (inputManager.isKeyPressed(GLFW_KEY_D))
		{
			move += rightVector;
		}
		//Forward/Backward Movement
		if (inputManager.isKeyPressed(GLFW_KEY_W))
		{
			move += forwardVector;
		}
		if (inputManager.isKeyPressed(GLFW_KEY_S)) 
		{
			move -= forwardVector;
		}

		//Jump Controller
		if (inputManager.isKeyPressed(GLFW_KEY_SPACE))
		{
			kinematicController->Jump(glm::vec3(0.0f, 5.0f, 0.0f));
		}

		if (glm::dot(move, move) > 0)
		{
			move = glm::normalize(move);
		}

		kinematicController->Walk(move * moveSpeed * deltaTime);
		owner->SetPosition(kinematicController->GetPosition());
	}

	void PlayerControllerComponent::SetIsActive(bool bActive)
	{
		bIsActive = bActive;
	}

	bool PlayerControllerComponent::GetIsActive()
	{
		return bIsActive;
	}

	bool PlayerControllerComponent::OnGround() const
	{
		if (kinematicController) //If we have a kinematic controller, returns its on ground call. If not return false as we have no physics (cannot leave ground/jump)
		{
			return kinematicController->OnGround();
		}
		return false;
	}

}