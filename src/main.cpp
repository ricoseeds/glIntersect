#include "../include/main.h"
#include "../include/stb_image.h"

int main(int argc, char *argv[])
{
    if (getOsName() == "Mac")
    {
        mac_moved = false;
    }
    int components;
    unsigned char *imageData = stbi_load("images/brain.jpeg", &gWindowWidth, &gWindowHeight, &components, STBI_rgb_alpha);
    gWindowWidth *= 1.5;
    gWindowHeight *= 1.5;
    if (imageData == NULL)
    {
        std::cerr << "Error loading texture '"
                  << "'" << std::endl;
        return false;
    }
    if (!initOpenGL())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }
    glfwSetCursorPosCallback(gWindow, cursorPositionCallback);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glCullFace(GL_BACK);
    //Load the Shaders
    ShaderProgram lightingShader, polyShader;
    lightingShader.loadShaders("shaders/vertx.vert", "shaders/fragh.frag");
    polyShader.loadShaders("shaders/poly.vert", "shaders/poly.frag");
    Texture2D texture;
    // texture.loadTexture("images/ducktomy.png", true);
    texture.loadTexture("images/brain.jpeg", true);
    //FONT INIT
    // // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader shader("shaders/font.vert", "shaders/font.frag"); // font shader

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(gWindowWidth), 0.0f, static_cast<GLfloat>(gWindowHeight));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set the required callback functions
    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
    glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

    // Define the viewport dimensions
    glViewport(0, 0, gWindowWidth, gWindowHeight);
    glEnable(GL_DEPTH_TEST);
    //FONT manipulaltion
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "/Users/arghachakraborty/Library/Fonts/OpenSans-Regular.ttf", 0, &face))
        // if (FT_New_Face(ft, "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48);
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)};
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Configure VAO/VBO for texture quads
    // GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //END OF FONT

    //Vertices for image

    // GLfloat symmetry_create1 = 0.4; //max is 1 for entire screen
    // static const GLfloat image_points[] = {
    //     -symmetry_create1, -symmetry_create1, 0,
    //     symmetry_create1, -symmetry_create1, 0,
    //     symmetry_create1, symmetry_create1, 0,
    //     0, symmetry_create1 - 0.2, 0,
    //     -symmetry_create1, symmetry_create1, 0};

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
    // Indexing
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(gWindow))
    {
        // showFPS(gWindow, shader);
        glfwPollEvents();
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Rest of the stuff
        // shader.use();
        showFPS(gWindow, shader);
        RenderText(shader, "STATISTICS:", 25.0f, 80.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, "No of Vertices GT:", 25.0f, 60.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, std::to_string(truth_data.size()), 25.0f, 40.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, "No of Vertices RAW:", 25.0f, 30.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, std::to_string(raw_data.size()), 25.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, "SCORE", gWindowWidth - 80, gWindowHeight - 20, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        if (computation_done)
        {

            RenderText(shader, std::to_string(score * 100), gWindowWidth - 80, gWindowHeight - 50, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        GLuint mVBO, mVAO;
        if (show_truth)
        {
            polyShader.use();

            polyShader.setUniform("mycolor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

            //Dynamic part involes drawing polygon

            glGenVertexArrays(1, &mVAO);
            glGenBuffers(1, &mVBO);
            glBindVertexArray(mVAO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            // glBufferData(GL_ARRAY_BUFFER, sizeof(image_points), image_points, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, truth_data.size() * sizeof(glm::vec3), &truth_data[0], GL_DYNAMIC_DRAW);
            // Vertex Positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
            //actual rendering of poly
            glBindVertexArray(mVAO);
            glDrawArrays(GL_LINE_LOOP, 0, truth_data.size());
            glBindVertexArray(0);

            //End Dynamic part involes drawing polygon
        }
        if (number_of_probe_points > 0 && show_data)
        {
            polyShader.use();
            polyShader.setUniform("mycolor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            //Dynamic part involes drawing polygon
            // GLuint mVBO, mVAO;
            glGenVertexArrays(1, &mVAO);
            glGenBuffers(1, &mVBO);
            glBindVertexArray(mVAO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            // glBufferData(GL_ARRAY_BUFFER, sizeof(image_points), image_points, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, raw_data.size() * sizeof(glm::vec3), &raw_data[0], GL_DYNAMIC_DRAW);
            // Vertex Positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
            //actual rendering of poly
            glBindVertexArray(mVAO);
            glDrawArrays(GL_LINE_LOOP, 0, raw_data.size());
            glBindVertexArray(0);

            //End Dynamic part involes drawing polygon
        }
        if (computation_done && show_intersection)
        {
            for (size_t i = 0; i < result_data.size(); i++)
            {
                // for (size_t j = 0; j < result_data[i].size(); j++)
                // {
                polyShader.use();
                polyShader.setUniform("mycolor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
                glGenVertexArrays(1, &mVAO);
                glGenBuffers(1, &mVBO);
                glBindVertexArray(mVAO);
                glBindBuffer(GL_ARRAY_BUFFER, mVBO);
                // glBufferData(GL_ARRAY_BUFFER, sizeof(image_points), image_points, GL_STATIC_DRAW);
                glBufferData(GL_ARRAY_BUFFER, result_data[i].size() * sizeof(glm::vec3), &result_data[i][0], GL_DYNAMIC_DRAW);
                // Vertex Positions
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *)0);
                glEnableVertexAttribArray(0);
                glBindVertexArray(0);
                //actual rendering of poly
                glBindVertexArray(mVAO);
                glDrawArrays(GL_LINE_LOOP, 0, result_data[i].size());
                glBindVertexArray(0);
                // }
            }
        }

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