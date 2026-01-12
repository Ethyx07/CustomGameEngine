#include "physics/RigidBody.h"
#include "Engine.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace eng
{
	
	RigidBody::RigidBody(BodyType bodyType, const std::shared_ptr<Collider>& bodyCollider, float bodyMass, float bodyFriction) :
		type(bodyType), collider(bodyCollider), mass(bodyMass), friction(bodyFriction)
	{
		if (!bodyCollider) //Ensures it has a valid collider or else doesnt create anything
		{
			return;
		}

		btVector3 inertia(0, 0, 0);
		if (type == BodyType::Dynamic && mass > 0.0f && collider->GetShape()) //Only objects with mass have local inertia. Also only dynamic body types are affected naturally (not scripted)
		{
			collider->GetShape()->calculateLocalInertia(btScalar(mass), inertia); //Calulates local inertia with vector3 as the output vector
		}
		
		btTransform transform;
		transform.setIdentity(); //Gets the starting state of the object and 
		btDefaultMotionState* motionState = new btDefaultMotionState(transform); //Used to interpolate transforms and sync physics & rendering

		btRigidBody::btRigidBodyConstructionInfo info((type == BodyType::Dynamic) ? btScalar(mass) : btScalar(0.0), //Sets mass as mass if type is dynamic else mass is 0
			motionState, collider->GetShape(), inertia);

		body = std::make_unique<btRigidBody>(info);
		body->setFriction(btScalar(friction));

		//Kinematic Only:
		if (type == BodyType::Kinematic)
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			body->setActivationState(DISABLE_DEACTIVATION);
		}
	}

	RigidBody::~RigidBody()
	{
		if (bIsAddedToWorld)
		{
			Engine::GetInstance().GetPhysicsManager().RemoveRigidBody(this);
		}
	}

	btRigidBody* RigidBody::GetBody()
	{
		return body.get();
	}

	BodyType RigidBody::GetType()
	{
		return type;
	}

	void RigidBody::SetAddedToWorld(bool added)
	{
		bIsAddedToWorld = added;
	}

	bool RigidBody::IsAddedToWorld()
	{
		return bIsAddedToWorld;
	}

	void RigidBody::SetPosition(const glm::vec3& pos) //Only works prior to adding the body to the world. If added to world, bullet simulation overrides it
	{
		if (!body)
		{
			return;
		}

		auto& trans = body->getWorldTransform();
		trans.setOrigin(btVector3(btScalar(pos.x), btScalar(pos.y), btScalar(pos.z))); //Sets the origin to the input pos

		if (body->getMotionState())
		{
			body->getMotionState()->setWorldTransform(trans);
		}
		body->setWorldTransform(trans);
	}

	glm::vec3 RigidBody::GetPosition() const
	{
		const auto& pos = body->getWorldTransform().getOrigin(); //Bullets world transform converted to glm::vec3
		return glm::vec3(pos.x(), pos.y(), pos.z());
	}

	void RigidBody::SetRotation(const glm::quat& rot)
	{
		if (!body)
		{
			return;
		}

		auto& trans = body->getWorldTransform();
		trans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w)); //Sets rotation quat to btQuaternion form

		if (body->getMotionState()) //Keeps the motion state in sync
		{
			body->getMotionState()->setWorldTransform(trans);
		}
		body->setWorldTransform(trans);
	}
	
	glm::quat RigidBody::GetRotation() const
	{
		const auto& rot = body->getWorldTransform().getRotation();
		return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
	}

	void RigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		body->applyCentralImpulse(btVector3(btScalar(impulse.x), btScalar(impulse.y), btScalar(impulse.z)));
	}

}