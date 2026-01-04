#pragma once

namespace eng
{

	class GameObject;
	class Component
	{
	public:
		virtual ~Component() = default;
		virtual void Init();
		virtual void Update(float deltaTime) = 0;
		virtual size_t GetTypeId() const = 0;
		
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

#define COMPONENT(ComponentClass) \
public: \
	static size_t TypeId() { return Component::StaticTypeId<ComponentClass>(); } \
	size_t GetTypeId() const override {return TypeId();}

}