#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Vec3 //Not using glm library
{
    float r;
    float g;
    float b;
};
struct Object
{
    float offsetX;
    float offsetY;
    float speed;
    int indicesCount; //Added indices count so we can change the shape from triangle to rectangle
    bool bAnimated;
    bool bControlled;
    Vec3 colour;
};

struct Vec2 
{
    float x = 0.0f;
    float y = 0.0f;
};

Vec2 offset;
Object objects[5] = 
{
    {0.0f, 0.0f, 1.0f, 3,false, false, {1.0f, 0.0f, 0.0f}},
    {0.5f, 0.0f, 1.0f, 3,false, false, {0.0f, 1.0f, 1.0f}},
    {-0.5f, 1.0f, 1.0f, 3,false, false, {0.0f, 0.5f, 1.0f}},
    {0.0f, 0.0f, 1.0f, 3,false, true, {1.0f, 0.0f, 1.0f}},
    {0.5f, 0.5f, 1.0f, 6,true, false, {0.5f, 0.3f, 0.7f}}
};

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) 
{
    /*if (action == GLFW_PRESS) 
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
    }*/
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
    

        out vec3 fragColor;
        uniform vec3 uColor;
        uniform vec2 uOffset;
       
        
        void main()
        {
            fragColor = uColor;
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

        in vec3 fragColor;

        void main()
        {
            FragColor = vec4(fragColor, 1.0f);
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
    
    float deltaTime = 0.0f; //Creates delta time and last time float
    float lastTime = 0.0f;

    while (!glfwWindowShouldClose(window))  //Loops until window is prompted to close
    {
        float t = (float)glfwGetTime();
        deltaTime = t - lastTime;
        lastTime = t;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        for (auto& obj : objects) 
        {
            float x = obj.offsetX;
            float y = obj.offsetY;

            if (obj.bAnimated)
            {
                obj.colour.g = 0.5f + (0.5f * sin(t));
                x = sin(glfwGetTime());
                y = tan(glfwGetTime());

            }
            if (obj.bControlled) //Key inputs move the triangle and update its new offset
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                {
                    x -= obj.speed * deltaTime;
                    obj.offsetX = x;
                }
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                {
                    x += obj.speed * deltaTime;
                    obj.offsetX = x;
                }
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                {
                    y += obj.speed * deltaTime;
                    obj.offsetY = y;
                }
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                {
                    y -= obj.speed * deltaTime; 
                    obj.offsetY = y;
                }
            }

            glUniform2f(uOffsetLoc, x, y);
            glUniform3f(uColorLoc, obj.colour.r, obj.colour.g, obj.colour.b);

            glDrawElements(GL_TRIANGLES, obj.indicesCount, GL_UNSIGNED_INT, nullptr);
        }
        glfwSwapBuffers(window); //Swaps buffers between back and front buffer
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}