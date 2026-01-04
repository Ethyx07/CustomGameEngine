#include "physics/PhysicsManager.h"
#include "physics/RigidBody.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace eng
{
	PhysicsManager::PhysicsManager()
	{

	}

	PhysicsManager::~PhysicsManager()
	{

	}

	void PhysicsManager::Init()
	{
		// -- World Initialisation -- //
		broadphase = std::make_unique<btDbvtBroadphase>();
		collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
		solver = std::make_unique<btSequentialImpulseConstraintSolver>();
		world = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfig.get());

		world->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	}

	void PhysicsManager::Update(float deltaTime)
	{
		const btScalar fixedTimeStep = 1.0f / 60.0f;
		const int maxSubsteps = 4;
		world->stepSimulation(deltaTime, maxSubsteps, fixedTimeStep);
	}

	btDiscreteDynamicsWorld* PhysicsManager::GetWorld()
	{ 
		return world.get(); 
	}

	void PhysicsManager::AddRigidBody(RigidBody* body)
	{
		if (!body || !world) //Makes sure body and the world are both valid
		{
			return;
		}

		if (auto* rigidBody = body->GetBody()) //Only if the rigid body contains a btRigidBody
		{
			world->addRigidBody(rigidBody, btBroadphaseProxy::StaticFilter, btBroadphaseProxy::AllFilter);
			body->SetAddedToWorld(true);
		}
	}

	void PhysicsManager::RemoveRigidBody(RigidBody* body)
	{
		if (!body || !world) //Makes sure body and the world are both valid
		{
			return;
		}

		if (auto* rigidBody = body->GetBody())
		{
			world->removeRigidBody(rigidBody);
			body->SetAddedToWorld(false);
		}
	}


}