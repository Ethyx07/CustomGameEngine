#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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