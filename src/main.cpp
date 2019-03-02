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
    Texture2D texture;
    texture.loadTexture("images/ducktomy.png", false);
    //Vertices for image

    GLfloat symmetry_create = 0.8; //max is 1 for entire screen
    // static const GLfloat image_points[] = {
    //     -symmetry_create, -symmetry_create, 0,
    //     symmetry_create, -symmetry_create, 0,
    //     symmetry_create, symmetry_create, 0,
    //     // 0, symmetry_create - 0.2, 0,
    //     -symmetry_create, symmetry_create, 0};
    static const GLfloat verties[] = {
        -symmetry_create,
        symmetry_create,
        0.0f,
        //uv
        0.0f, 1.0f,
        symmetry_create,
        symmetry_create,
        0.0f,
        //uv
        1.0f, 1.0f,
        symmetry_create,
        -symmetry_create,
        0.0f,
        //uv
        1.0f, 0.0f,
        -symmetry_create,
        -symmetry_create,
        0.0f,
        //uv
        0.0f, 0.0f};
    GLuint indeces[] = {
        0, 1, 2,
        0, 2, 3};
    GLuint vbo, ibo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verties), verties, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //tell vertex shader how vertices are laid out

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    //texCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //Indexing
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    // Game loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);
        glfwPollEvents();
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Rest of the stuff
        lightingShader.use();
        //code
        // GLuint mVBO, mVAO;

        // glGenVertexArrays(1, &mVAO);

        // glGenBuffers(1, &mVBO);

        // glBindVertexArray(mVAO);
        // glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(image_points), image_points, GL_STATIC_DRAW);

        // // Vertex Positions
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        // glEnableVertexAttribArray(0);
        // glBindVertexArray(0);
        // //code
        // glBindVertexArray(mVAO);
        // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        // glBindVertexArray(0);
        //Static draw part
        texture.bind(0);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        texture.unbind(0);
        //End of Static draw part

        glfwSwapBuffers(gWindow);
        mac_patch(gWindow);
    }
    glfwTerminate();

    return 0;
}
