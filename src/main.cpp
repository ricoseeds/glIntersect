#include "../include/main.h"

int main(int argc, char *argv[])
{
    if (getOsName() == "Mac")
    {
        mac_moved = false;
    }
    if (!initOpenGL())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }
    // Game loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);
        glfwPollEvents();
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Rest of the stuff
        glfwSwapBuffers(gWindow);
        mac_patch(gWindow);
    }
    glfwTerminate();

    return 0;
}
