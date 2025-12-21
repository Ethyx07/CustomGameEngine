#include "TestObjectOrbit.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

TestObjectOrbit::TestObjectOrbit()
{
}

void TestObjectOrbit::Update(float deltaTime)
{
    eng::GameObject::Update(deltaTime);

    float rotAngle = glm::radians(90.0f);
    glm::quat delta = glm::angleAxis(rotAngle * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));

    SetRotation(glm::normalize(delta * GetRotation()));
    
}