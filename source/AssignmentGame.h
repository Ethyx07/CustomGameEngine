#pragma once
#include <eng.h>

class AssignmentGame : public eng::Application
{
public:
	bool Init() override;
	void Update(float deltaTime) override;
	void Destroy() override;

private:
	eng::Material material;
	eng::Material material2;
	std::unique_ptr<eng::Mesh> mesh;
	float offsetX = 0;
	float offsetY = 0;
	
	eng::Scene scene;
};