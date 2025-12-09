#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;


struct Vec2 
{
    float x = 0.0f;
    float y = 0.0f;
};

Vec2 offset;

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) 
{
    if (action == GLFW_PRESS) 
    {
        switch (key) 
        {
        case GLFW_KEY_UP:
            offset.y += 0.01f; break;
        case GLFW_KEY_DOWN:
            offset.y -= 0.01f; break;
        case GLFW_KEY_RIGHT:
            offset.x += 0.01f; break;
        case GLFW_KEY_LEFT:
            offset.x -= 0.01f; break;
        default: break;
        }
    }
}

int main()
{
    if (!glfwInit()) //Makes sure glfw initialised properly
    {
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //tells glfw what version of opengl is being used and sets its profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   
    GLFWwindow* window = glfwCreateWindow(1280, 720, "GameDevelopmentProject", NULL, NULL); //Creates window terminal
    if (window == NULL) 
    {
        cout << "ERROR CREATING WINDOW" << endl;
        glfwTerminate(); //Terminates if window is not created
        return -1;
    }
    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window); //Sets window as glfw current context

    if (glewInit() != GLEW_OK)  //Checks if glew was initialised properly
    {
        glfwTerminate();
        return -1;
    }

    string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        uniform vec2 uOffset;

        out vec3 vColor;
        
        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
        }
    )";

    //Creates and compiles vertex shader, checks if if was compiled properly //
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        cerr << "ERROR:VERTEX SHADER COMPILATION FAILED: " << infoLog << endl;
    }

    string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;
        uniform vec4 uColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0f) * uColor;
        }
    )";

    //Compiles fragmentShader and checks if it compiled properly //
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCStr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        cerr << "ERROR: FRAGMENT SHADER COMPILATION FAILED: " << infoLog << endl;
    }
   //Creates shader program and checks if it properly linked
   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if (!success)
   {
       char infoLog[512];
       glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
       cerr << "ERROR: SHADER PROGRAM LINK FAILED: " << infoLog << endl;
   }
   //Deletes shaders as they are now internally stored after the link
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

    vector<float> vertices = 
    {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };
    
    vector<unsigned int> indices = 
    {
        0,1,2,
        0,2,3
    };

    //Creates VBO binds it, passes data and unbinds
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //ARRAY_BUFFER tells that it will store an array of data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbinds buffer but vertex data is already stored

    //Creates ebo and buffers the data of indices before unbinding
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Creates vao, binds it and passes data then unbinds
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0); //Location, number of components ,data type, stride value, offset
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float))); //Stride value is 6 as there are 6 values per vertex (3 pos, 3 colour)
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");
    GLint uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");    

    while (!glfwWindowShouldClose(window))  //Loops until window is prompted to close
    {
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform4f(uColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
        glUniform2f(uOffsetLoc, offset.x, offset.y);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUniform4f(uColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window); //Swaps buffers between back and front buffer
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}