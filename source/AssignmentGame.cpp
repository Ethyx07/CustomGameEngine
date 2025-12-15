#include "AssignmentGame.h"
#include "TestObject.h"
#include "GLFW/glfw3.h"
#include <iostream>

bool AssignmentGame::Init()
{
    scene.CreateObject<TestObject>("TestObject");
    return true;
}

void AssignmentGame::Update(float deltaTime)
{
    scene.Update(deltaTime);
}

void AssignmentGame::Destroy()
{
   
}