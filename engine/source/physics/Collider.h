#pragma once
#include <glm/vec3.hpp>

class btCollisionShape;

namespace eng
{
	class Collider
	{

	public:
		virtual ~Collider();
		btCollisionShape* GetShape() const;

	protected:
		btCollisionShape* shape = nullptr;

	};

	class BoxCollider : Collider
	{
	public:

		BoxCollider(const glm::vec3& extents);

	};

	class SphereCollider : Collider
	{
	public:

		SphereCollider(float radius);

	};

	class CapsuleCollider : Collider
	{
	public:

		CapsuleCollider(float radius, float height);

	};
}