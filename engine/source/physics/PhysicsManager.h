#pragma once
#include <memory>

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace eng
{
	class RigidBody;

	class PhysicsManager
	{
	public:
		PhysicsManager();
		~PhysicsManager();

		void Init();
		void Update(float deltaTime);

		btDiscreteDynamicsWorld* GetWorld();

		void AddRigidBody(RigidBody* body);
		void RemoveRigidBody(RigidBody* body);

	private:
		std::unique_ptr<btBroadphaseInterface> broadphase;
		std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig;
		std::unique_ptr<btCollisionDispatcher> dispatcher;
		std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
		std::unique_ptr<btDiscreteDynamicsWorld> world;

	};
}