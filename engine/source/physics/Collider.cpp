#include "physics/Collider.h"

#include <btBulletCollisionCommon.h>

namespace eng
{
	 Collider::~Collider() 
	{ 
		 if (shape)
		 {
			 delete shape;
		 }
	}

	btCollisionShape* Collider::GetShape() const 
	{ 
		return shape; 
	}

	BoxCollider::BoxCollider(const glm::vec3& extents)
	{
		glm::vec3 halfExtent = extents * 0.5f;
		shape = new btBoxShape(btVector3(halfExtent.x, halfExtent.y, halfExtent.z));
	}

	SphereCollider::SphereCollider(float radius)
	{
		shape = new btSphereShape(btScalar(radius));
	}

	CapsuleCollider::CapsuleCollider(float radius, float height)
	{
		shape = new btCapsuleShape(btScalar(radius), btScalar(height));
	}
}
