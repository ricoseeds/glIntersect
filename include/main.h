#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

//Globals
const char *APP_TITLE = "Internet-enabled Skills Training";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow *gWindow = NULL;
glm::vec4 gClearColor(0.0f, 0.0f, 0.0f, 1.0f);
static bool mac_moved = false;

// Function Prototypes
void mac_patch(GLFWwindow *window);
// Callbacks
void glfw_onFramebufferSize(GLFWwindow *window, int width, int height);
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);

// boiler plate for setting up opengl
bool initOpenGL()
{
    // Intialize GLFW
    // GLFW is configured.  Must be called before calling any GLFW functions
    if (!glfwInit())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

    // Create an OpenGL 3.3 core, forward compatible context window
    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
    if (gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(gWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Set the required callback functions
    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);

    glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

    // Define the viewport dimensions
    glViewport(0, 0, gWindowWidth, gWindowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void mac_patch(GLFWwindow *window)
{
    if (glfwGetTime() > 1.0)
    {
        mac_moved = true;
    }
    // glfwGetTim

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE && !mac_moved)
    {
        int x, y;
        glfwGetWindowPos(window, &x, &y);
        glfwSetWindowPos(window, ++x, y);
    }
    else
    {
        mac_moved = true;
    }
}

void showFPS(GLFWwindow *window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

    elapsedSeconds = currentSeconds - previousSeconds;

    // Limit text updates to 4 times per second
    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        // The C++ way of setting the window title
        std::ostringstream outs;
        outs.precision(3); // decimal places
        outs << std::fixed
             << APP_TITLE << "    "
             << "FPS: " << fps << "    "
             << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        // Reset for next average.
        frameCount = 0;
    }

    frameCount++;
}
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void glfw_onFramebufferSize(GLFWwindow *window, int width, int height)
{
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, gWindowWidth, gWindowHeight);
}