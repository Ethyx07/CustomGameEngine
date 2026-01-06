#include "Game.h"
#include "TestObject.h"
#include "TestObjectOrbit.h"
#include "Player.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>




bool Game::Init()
{
    auto& fs = eng::Engine::GetInstance().GetFileSystem();
    auto texture = eng::Texture::Load("textures/brick.png");

    scene = new eng::Scene();
	eng::Engine::GetInstance().SetScene(scene);

	auto player = scene->CreateObject<Player>("Player");
	player->Init();
	scene->SetMainCamera(player);

    auto material = eng::Material::Load("materials/brick.mat");

	auto mesh = eng::Mesh::CreateBox();

    auto objectA = scene->CreateObject("ObjectA");
    objectA->AddComponent(new eng::MeshComponent(material, mesh));
    objectA->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectB = scene->CreateObject("ObjectB");
    objectB->AddComponent(new eng::MeshComponent(material, mesh));
    objectB->SetPosition(glm::vec3(-10.0f, 2.0f, 2.0f));
    objectB->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectC = scene->CreateObject("ObjectC");
    objectC->AddComponent(new eng::MeshComponent(material, mesh));
    objectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objectC->SetRotation(glm::vec3(1.0f, 0.0f, 1.0f));
    objectC->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

	auto suzanneObj = eng::GameObject::LoadGLTF("models/suzanne/Suzanne.gltf");
	suzanneObj->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

	auto ground = scene->CreateObject("ground");
	ground->SetPosition(glm::vec3(0, -5, 0));
	glm::vec3 groundExtents(20, 2, 20);
	auto groundMesh = eng::Mesh::CreateBox(groundExtents);
	ground->AddComponent(new eng::MeshComponent(material, groundMesh));

	auto groundCollider = std::make_shared<eng::BoxCollider>(groundExtents);
	auto groundBody = std::make_shared<eng::RigidBody>(eng::BodyType::Static, groundCollider, 0.0f, 0.5f);

	ground->AddComponent(new eng::PhysicsComponent(groundBody));

	auto boxObj = scene->CreateObject("fallingBox");
	boxObj->AddComponent(new eng::MeshComponent(material, mesh));

	boxObj->SetPosition(glm::vec3(0, 2, 2));
	boxObj->SetRotation(glm::quat(glm::vec3(1, 2, 0)));

	auto boxCollider = std::make_shared<eng::BoxCollider>(glm::vec3(1));
	auto boxBody = std::make_shared<eng::RigidBody>(eng::BodyType::Dynamic, boxCollider, 5.0f, 0.5f);

	boxObj->AddComponent(new eng::PhysicsComponent(boxBody));

	
    auto light = scene->CreateObject("Light");
    auto lightComp = new eng::LightComponent();
    lightComp->SetColour(glm::vec3(1.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

    

    return true;
}

void Game::Update(float deltaTime)
{	
	scene->Update(deltaTime);

	time += deltaTime;
	fpsCounter += 1;
	timeSinceLastSecond += deltaTime; //Basic fps counter that adds the fps each update until its been a second and then prints and resets
	if (timeSinceLastSecond >= 1.0f) {
		std::cout << "FPS: " << fpsCounter << std::endl;
		fpsCounter = 0;
		timeSinceLastSecond = 0;
	}
}

void Game::Destroy()
{

}