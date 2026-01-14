#include "DiscoLight.h"

void DiscoLight::Update(float deltaTime)
{
	colourTime += deltaTime;
	float red = abs(sin(colourTime));
	float blue = abs(cos(colourTime));
	float green = abs(sin(colourTime));

	glm::vec3 colour(red, green, blue);
	SetColour(colour);
}
