#include "physics/PhysicsManager.h"
#include "physics/RigidBody.h"
#include "physics/CollisionObject.h"
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

		//Process collisions logic
		auto dispatcher = world->getDispatcher();
		const auto numManifolds = dispatcher->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* manifold = dispatcher->getManifoldByIndexInternal(i);

			auto bodyA = reinterpret_cast<CollisionObject*>(manifold->getBody0()->getUserPointer()); //Gets one of the two colliding bodies
			auto bodyB = reinterpret_cast<CollisionObject*>(manifold->getBody1()->getUserPointer()); //Gets the other colliding body

			if (!bodyA || !bodyB)
			{
				continue; //Skips this iteration of if bodyA or bodyB is nullptr
			}

			for (int j = 0; j < manifold->getNumContacts(); j++)
			{
				const btManifoldPoint& point = manifold->getContactPoint(j); //Gets the points of contact for this manifold

				glm::vec3 pos(point.getPositionWorldOnB().x(), point.getPositionWorldOnB().y(), point.getPositionWorldOnB().z()); //Where the points collide
				glm::vec3 normal(point.m_normalWorldOnB.x(), point.m_normalWorldOnB.y(), point.m_normalWorldOnB.z()); //How the collision occurs (direction, etc)
				
				bodyA->DispatchContactEvent(bodyB, pos, normal); //Tells bodyA it collided with bodyB at position pos with a unit vector of normal
				bodyB->DispatchContactEvent(bodyA, pos, normal); //Same as above
			}
		}
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