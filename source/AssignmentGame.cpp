#include "AssignmentGame.h"
#include "GLFW/glfw3.h"
#include <iostream>

bool AssignmentGame::Init()
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
        uniform float uGreen;
        
        void main()
        {
            vColor = vec3(0.5f, uGreen, 0.7f);
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

	auto& graphicsAPI = eng::Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    auto shaderProgram1 = graphicsAPI.CreateShaderProgram(vertexShaderSource1, fragmentShaderSource);

    material.SetShaderProgram(shaderProgram);
    material2.SetShaderProgram(shaderProgram1);

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

    mesh = std::make_unique<eng::Mesh>(vertexLayout, vertices, indices); //Creates unique mesh that has the vertices and indices above as well as the vertex layout

    return true;
}

void AssignmentGame::Update(float deltaTime)
{
    time += deltaTime;
    fpsCounter += 1;
    timeSinceLastSecond += deltaTime; //Basic fps counter that adds the fps each update until its been a second and then prints and resets
    if (timeSinceLastSecond >= 1.0f) {
        std::cout << "FPS: " << fpsCounter << std::endl;
        fpsCounter = 0;
        timeSinceLastSecond = 0;
    }
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

    
    eng::RenderCommand command;

    if (bToggleCooldown) { //Creates a toggle cooldown for changing materials since this is called every frame
        currentCooldown += deltaTime;
        if (currentCooldown >= toggleCooldown) {
            bToggleCooldown = false;
            currentCooldown = 0.0f;
        }
    }

    if (input.isKeyPressed(GLFW_KEY_SPACE) && !bToggleCooldown) { //Changes what material is being applied to the mesh
        bToggle = !bToggle; //Swaps toggle around
        bToggleCooldown = true;
    }
    if (bToggle) {
        float greenOffset = 0.5f + (0.5f * sin(time));
        material2.SetParam("uOffset", offsetX, offsetY);
        material2.SetParam("uGreen", greenOffset);
        command.material = &material2;
    }
    else {
        material.SetParam("uOffset", offsetX, offsetY);
        command.material = &material;
    }
    command.mesh = mesh.get();

    auto& renderQueue = eng::Engine::GetInstance().GetRenderQueue();
    renderQueue.Submit(command);

}

void AssignmentGame::Destroy()
{
   
}