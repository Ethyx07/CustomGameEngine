#pragma once
#include <eng.h>


class TestObject : public eng::GameObject
{
public:
	TestObject();

	void Update(float deltaTime) override;

private:
	eng::Material material;
	std::shared_ptr<eng::Mesh> mesh;
	float offsetX = 0;
	float offsetY = 0;
};