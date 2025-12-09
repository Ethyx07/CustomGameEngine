#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

    glfwMakeContextCurrent(window); //Sets window as glfw current context

    if (glewInit() != GLEW_OK)  //Checks if glew was initialised properly
    {
        glfwTerminate();
        return -1;
    }

    string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        
        void main()
        {
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
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

        void main()
        {
            FragColor = vec4(1.0f ,0.0f, 0.0f, 1.0f);
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
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f
    };

    while (!glfwWindowShouldClose(window))  //Loops until window is prompted to close
    {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window); //Swaps buffers between back and front buffer
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}