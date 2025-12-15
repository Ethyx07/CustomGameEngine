#include "scene/Component.h"

namespace eng
{
	GameObject* Component::GetOwner()
	{
		return owner;
	}
}