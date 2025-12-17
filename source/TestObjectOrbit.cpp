#include "TestObjectOrbit.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

TestObjectOrbit::TestObjectOrbit()
{
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        void main()
        {
            vColor = color;
            gl_Position = uProjection * uView * uModel * vec4(position, 1.0f);
        }
    )";

    std::string vertexShaderSource1 = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        uniform mat4 uModel;
        uniform float uGreen;
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        void main()
        {
            vColor = vec3(0.5f, uGreen, 0.7f);
            gl_Position = uProjection * uView * uModel * vec4(position, 1.0f);
        }
    )";

    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;
     

        void main()
        {
            FragColor = vec4(vColor, 1.0f);
        }
    )";

    auto& graphicsAPI = eng::Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    auto shaderProgram1 = graphicsAPI.CreateShaderProgram(vertexShaderSource1, fragmentShaderSource);

    auto material = std::make_shared<eng::Material>();
    auto material2 = std::make_shared < eng::Material>();
    material->SetShaderProgram(shaderProgram);
    material2->SetShaderProgram(shaderProgram1);

    std::vector<float> vertices =
    {
        //Front face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        //Back Face
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f
    }; //Creates vertices to be used for vertex buffer

    std::vector<unsigned int> indices =
    {
        //Front face
        0,1,2,
        0,2,3,
        //Top face
        4,5,1,
        4,1,0,
        //Right Face
        4,0,3,
        4,3,7,
        //Left Face
        1,5,6,
        1,6,2,
        //Bottom Face
        3,2,6,
        3,6,7,
        //Back Face
        4,7,6,
        4,6,5

    }; //Creates indices to be used for index buffer

    eng::VertexLayout vertexLayout;
    vertexLayout.elements.push_back({
        0,
        3,
        GL_FLOAT,
        0
        });

    vertexLayout.elements.push_back({
        1,
        3,
        GL_FLOAT,
        sizeof(float) * 3
        });

    vertexLayout.stride = sizeof(float) * 6;


   auto mesh = std::make_shared<eng::Mesh>(vertexLayout, vertices, indices); //Creates unique mesh that has the vertices and indices above as well as the vertex layout

   AddComponent(new eng::MeshComponent(material, mesh));
}

void TestObjectOrbit::Update(float deltaTime)
{
    eng::GameObject::Update(deltaTime);

    float rotAngle = glm::radians(90.0f);
    glm::quat delta = glm::angleAxis(rotAngle * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));

    SetRotation(glm::normalize(delta * GetRotation()));
    
}