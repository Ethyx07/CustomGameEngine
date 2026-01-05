#include "physics/KinematicCharacterController.h"
#include "Engine.h"

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace eng
{
	KinematicCharacterController::KinematicCharacterController(float kinRadius, float kinHeight)
		: height(kinHeight), radius(kinRadius)
	{
		auto world = Engine::GetInstance().GetPhysicsManager().GetWorld(); //Gets world instance
		auto capsule = new btCapsuleShape(radius, height); //Creates capsule comp for kinematic controller. (Standard shape for characters)

		ghost = std::make_unique<btPairCachingGhostObject>();
		btTransform start;
		start.setIdentity();
		start.setOrigin(btVector3(0.0f, 2.0f, 0.0f)); //Makes sure that origin is above the ground (0 y value)

		ghost->setWorldTransform(start);
		ghost->setCollisionShape(capsule);
		ghost->setCollisionFlags(ghost->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);

		world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

		const btScalar stepHeight = 0.35f; //Sets the step up height of the controller

		controller = std::make_unique<btKinematicCharacterController>(ghost.get(), capsule, stepHeight);

		controller->setMaxSlope(btRadians(50)); //Sets the max slope the player can move up
		controller->setGravity(world->getGravity()); //Sets the gravity value of the player

		world->addCollisionObject(ghost.get(), btBroadphaseProxy::CharacterFilter, 
			btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
		world->addAction(controller.get());
	}

	KinematicCharacterController::~KinematicCharacterController()
	{
		auto world = Engine::GetInstance().GetPhysicsManager().GetWorld();
		if (controller)
		{
			world->removeAction(controller.get()); //Removes controller from world
		}
		if (ghost)
		{
			world->removeCollisionObject(ghost.get()); //Removes ghost from world
		}
	}

	glm::vec3 KinematicCharacterController::GetPosition() const
	{
		const auto& pos = ghost->getWorldTransform().getOrigin();

		//Offset upwards as camera is not at the centre of the capsule
		const glm::vec3 offset(0.0f, height * 0.5f + radius, 0.0f);
		
		return glm::vec3(pos.x(), pos.y(), pos.z()) + offset;
	}

	glm::quat KinematicCharacterController::GetRotation() const
	{
		const auto& rot = ghost->getWorldTransform().getRotation();
		return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
	}

	void KinematicCharacterController::Walk(const glm::vec3& direction)
	{
		controller->setWalkDirection(btVector3(direction.x, direction.y, direction.z));
	}

	void KinematicCharacterController::Jump(const glm::vec3& direction)
	{
		if (controller->onGround())
		{
			controller->jump(btVector3(direction.x, direction.y, direction.z));
		}
	}

	bool KinematicCharacterController::OnGround() const
	{
		return controller->onGround();
	}
}