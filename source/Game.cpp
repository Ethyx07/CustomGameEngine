#include "Game.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

int loops = 0;

bool Game::Init()
{
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        uniform vec2 uOffset;
        
        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
        }
    )";

    std::string vertexShaderSource1 = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        uniform vec2 uOffset;
        
        void main()
        {
            vColor = vec3(color.g, color.r, color.b);
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
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
    
    auto& graphicsAPI = eng::Engine::GetInstance().GetGraphicsAPI(); //Gets our graphics API from our engine instance
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource); //Creates a shader program using the graphicsAPI
    auto shaderProgram2 = graphicsAPI.CreateShaderProgram(vertexShaderSource1, fragmentShaderSource);
    material.SetShaderProgram(shaderProgram);
    material2.SetShaderProgram(shaderProgram2);

    std::vector<float> vertices =
    {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    }; //Creates vertices to be used for vertex buffer

    std::vector<unsigned int> indices =
    {
        0,1,2,
        0,2,3
    }; //Creates indices to be used for index buffer

    eng::VertexLayout vertexLayout;
    //Position IN from vertex shader
    vertexLayout.elements.push_back({
        0,
        3,
        GL_FLOAT,
        0
        });

    //Colour IN from vertex shader
    vertexLayout.elements.push_back({
        1,
        3,
        GL_FLOAT,
        sizeof(float) * 3
        });
    vertexLayout.stride = sizeof(float) * 6;

    mesh = std::make_unique<eng::Mesh>(vertexLayout, vertices, indices); //Creates unique mesh that has the vertices and indices above as well as the vertex layout

	return true;
}

void Game::Update(float deltaTime)
{	
	auto& input = eng::Engine::GetInstance().GetInputManager();
	//Horizontal Movement
    if (input.isKeyPressed(GLFW_KEY_A))
	{
        offsetX -= 1.0f * deltaTime;
	}
    if (input.isKeyPressed(GLFW_KEY_D)) {
        offsetX += 1.0f * deltaTime;
    }
    //Vertical Movement
    if (input.isKeyPressed(GLFW_KEY_W))
    {
        offsetY += 1.0f * deltaTime;
    }
    if (input.isKeyPressed(GLFW_KEY_S)) {
        offsetY -= 1.0f * deltaTime;
    }
    

    material.SetParam("uOffset", offsetX, offsetY);
    eng::RenderCommand command;
    if (input.isKeyPressed(GLFW_KEY_SPACE)) {
        command.material = &material2;
    }
    else
    {
        command.material = &material;
    }
    
    command.mesh = mesh.get();

    auto& renderQueue = eng::Engine::GetInstance().GetRenderQueue();
    renderQueue.Submit(command);
}

void Game::Destroy()
{

}