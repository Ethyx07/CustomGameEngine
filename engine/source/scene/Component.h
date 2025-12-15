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

		template<typename T>
		static size_t StaticTypeId() //Gives each class its own id. (Diff component classes get diff ids)
		{
			static size_t typeId = nextId++;
			return typeId;
		}

	protected:
		GameObject* owner = nullptr;

		friend class GameObject;

	private:
		static size_t nextId;
	};
}