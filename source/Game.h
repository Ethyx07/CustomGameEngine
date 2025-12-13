#pragma once
#include <eng.h>
#include <memory>

class Game : public eng::Application
{
public:
	bool Init() override;
	void Update(float deltaTime) override;
	void Destroy() override;

private:
	eng::Material material;

	std::unique_ptr<eng::Mesh> mesh;
	float offsetX = 0;
	float offsetY = 0;
};