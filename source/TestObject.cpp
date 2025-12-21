#include "TestObject.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

TestObject::TestObject()
{
   
}

void TestObject::Update(float deltaTime)
{
    eng::GameObject::Update(deltaTime);

    float rotAngle = glm::radians(90.0f);
    glm::quat delta = glm::angleAxis(rotAngle * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));

    SetRotation(glm::normalize(delta * GetRotation()));

    auto position = GetPosition();
    

}