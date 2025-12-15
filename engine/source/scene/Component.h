#pragma once

namespace eng
{

	class GameObject;
	class Component
	{
	public:
		virtual ~Component() = default;
		virtual void Update(float deltaTime) = 0;
		
		GameObject* GetOwner();

	protected:
		GameObject* owner = nullptr;

		friend class GameObject;
	};
}