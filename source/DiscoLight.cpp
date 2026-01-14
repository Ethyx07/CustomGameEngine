#include "DiscoLight.h"
#include "eng.h"
#include "GLFW/glfw3.h"

void DiscoLight::Update(float deltaTime)
{
	if (bCoolingDown)
	{
		currentTime += deltaTime;
		if (currentTime >= cooldownTime)
		{
			bCoolingDown = false;
			currentTime = 0.0f;
		}
	}
	auto& input = eng::Engine::GetInstance().GetInputManager();
	if (input.isKeyPressed(GLFW_KEY_P) && !bCoolingDown)
	{
		SetDiscoMode(!GetDiscoMode());
		bCoolingDown = true;
	}
	glm::vec3 colour(1.0f);
	if (bDiscoMode)
	{
		colourTime += deltaTime;
		float red = abs(sin(colourTime));
		float blue = abs(cos(colourTime));
		float green = abs(sin(colourTime));

		colour = glm::vec3(red, green, blue);
	}
	
	SetColour(colour);
}

void DiscoLight::SetDiscoMode(bool disco)
{
	bDiscoMode = disco;
}


bool DiscoLight::GetDiscoMode()
{
	return bDiscoMode;
}