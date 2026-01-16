#include "scene/components/ui/CanvasComponent.h"
#include "scene/components/ui/UIElementComponent.h"
#include "scene/GameObject.h"

namespace eng
{
	void CanvasComponent::Update(float deltaTime)
	{
		const auto children = owner->GetChildren(); //Gets array of children pointers
		for (const auto& child : children)
		{
			if (auto component = child->GetComponent<UIElementComponent>()) //Gets component if child object has it
			{
				Render(component); //Renders each component that is of type UIElementComponent
			}
		}
	}

	void CanvasComponent::Render(UIElementComponent* element)
	{
		if (!element)
		{
			return;
		}
		//Render this element
		element->Render(this); //Calls the elements render function, passing in this canvas

		//Recursively renders all children that are also UI elements
		const auto children = element->GetOwner()->GetChildren();
		for (const auto& child : children)
		{
			if (auto component = child->GetComponent<UIElementComponent>())
			{
				Render(component);
			}
		}
	}
}