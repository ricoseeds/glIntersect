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
    //Load the Shaders
    ShaderProgram lightingShader;
    lightingShader.loadShaders("shaders/vertx.vert", "shaders/fragh.frag");
    //Vertices for image

    GLfloat symmetry_create = 0.5; //max is 1 for entire screen
    static const GLfloat image_points[] = {
        -symmetry_create, -symmetry_create, 0,
        symmetry_create, -symmetry_create, 0,
        symmetry_create, symmetry_create, 0,
        -symmetry_create, symmetry_create, 0};
    // Game loop
    GLuint mVBO, mVAO;

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // glBufferData(GL_ARRAY_BUFFER, image_points.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(image_points), image_points, GL_STATIC_DRAW);

    // Vertex Positions
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);
        glfwPollEvents();
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Rest of the stuff
        lightingShader.use();
        glBindVertexArray(mVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(gWindow);
        mac_patch(gWindow);
    }
    glfwTerminate();

    return 0;
}
