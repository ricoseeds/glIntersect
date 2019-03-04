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
    ShaderProgram lightingShader, polyShader;
    lightingShader.loadShaders("shaders/vertx.vert", "shaders/fragh.frag");
    polyShader.loadShaders("shaders/poly.vert", "shaders/poly.frag");
    Texture2D texture;
    texture.loadTexture("images/ducktomy.png", true);
    //Vertices for image

    GLfloat symmetry_create1 = 0.4; //max is 1 for entire screen
    static const GLfloat image_points[] = {
        -symmetry_create1, -symmetry_create1, 0,
        symmetry_create1, -symmetry_create1, 0,
        symmetry_create1, symmetry_create1, 0,
        0, symmetry_create1 - 0.2, 0,
        -symmetry_create1, symmetry_create1, 0};

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

    // Enable blending
    glEnable(GL_BLEND);
    // glBlendFunc(0.5, 0.5);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    // Game loop
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);
        glfwPollEvents();
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Rest of the stuff
        // lightingShader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        polyShader.use();
        polyShader.setUniform("mycolor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        //Dynamic part involes drawing polygon
        GLuint mVBO, mVAO;
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(image_points), image_points, GL_STATIC_DRAW);
        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        //actual rendering of poly
        glBindVertexArray(mVAO);
        // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawArrays(GL_LINE_LOOP, 0, 5);

        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //End Dynamic part involes drawing polygon

        //Static draw part
        lightingShader.use();
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
