#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/ShaderProgram.h"
#include "../include/Shader.h"
#include "../include/Texture2D.h"

#include <deque>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/correct.hpp>
#include <boost/foreach.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

//Globals
const char *APP_TITLE = "Internet-enabled Skills Training";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow *gWindow = NULL;
glm::vec4 gClearColor(1.0f, 0.0f, 0.0f, 1.0f);
static bool mac_moved = true;
bool close_truth = false;
double fps;
GLfloat symmetry_create = 1.0;
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
double normalizedX, normalizedY;
int number_of_truth_points = 0;
std::vector<glm::vec3> truth_data;
std::vector<glm::vec3> truth_data_real;
int number_of_probe_points = 0;
std::vector<glm::vec3> raw_data;
std::vector<glm::vec3> raw_data_real;
bool start_draw = false;
bool computation_done = false;
struct Character
{
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Offset to advance to next glyph
};
GLuint VAO, VBO; //for the fonts
std::map<GLchar, Character> Characters;
std::vector<std::vector<glm::vec3>> result_data;
// Function Prototypes

std::string getOsName();
void mac_patch(GLFWwindow *window);

// Callbacks
void glfw_onFramebufferSize(GLFWwindow *window, int width, int height);
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);
static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

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
    // Set OpenGL options

    return true;
}

void mac_patch(GLFWwindow *window)
{
    if (glfwGetTime() > 1.0)
    {
        mac_moved = true;
    }
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

void showFPS(GLFWwindow *window, Shader &shader)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

    elapsedSeconds = currentSeconds - previousSeconds;
    double fps_old;
    // double fps;
    // Limit text updates to 4 times per second
    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        fps = (double)frameCount / elapsedSeconds;
        // double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;
        fps_old = fps;
        // The C++ way of setting the window title
        std::ostringstream outs;
        outs.precision(3); // decimal places
        outs << std::fixed
             << APP_TITLE << "    "
             << "FPS: " << fps << "    "
             << "Frame Time: " << msPerFrame << " (ms)";

        // Reset for next average.
        frameCount = 0;
    }
    RenderText(shader, "FPS:", 25.0f, 100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    RenderText(shader, std::to_string((int)fps), 80.0f, 100.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

    frameCount++;
}
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        // close_truth = !close_truth;
        close_truth = true;
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        typedef boost::geometry::model::d2::point_xy<double> point;
        typedef boost::geometry::model::polygon<point> polygon;

        //First Polygon
        polygon poly;

        for (size_t i = 0; i < truth_data_real.size(); i++)
        {
            poly.outer().push_back(point(truth_data_real[i].x, truth_data_real[i].y));
        }

        // std::string reason;
        // bool first_valid = boost::geometry::is_valid(poly, reason);
        // std::cout << boost::geometry::dsv(poly) << " - " << (!first_valid ? "invalid (" + reason + ")" : "valid") << "\n";
        boost::geometry::correct(poly);
        // std::cout << boost::geometry::dsv(poly) << " - " << (!first_valid ? "invalid (" + reason + ")" : "valid") << "\n";

        polygon poly2;

        for (size_t i = 0; i < raw_data_real.size(); i++)
        {
            poly2.outer().push_back(point(raw_data_real[i].x, raw_data_real[i].y));
        }
        boost::geometry::correct(poly2);

        std::deque<polygon> output;
        boost::geometry::intersection(poly, poly2, output);
        int k = 0;
        //finds intersections
        std::cout << "INTERSECTION\n";
        BOOST_FOREACH (polygon const &p, output)
        {
            std::cout << k++ << ": " << boost::geometry::wkt(p) << std::endl;
            polygon new_intersection_output = p;
            std::vector<point> const &points = new_intersection_output.outer();
            // std::vector<double> Px;
            // std::vector<double> Py;
            std::vector<glm::vec3> extract_points;
            for (std::vector<point>::size_type i = 0; i < points.size(); ++i)
            {
                // Px.push_back(boost::geometry::get<0>(points[i]));
                // Py.push_back(boost::geometry::get<1>(points[i]));
                double px = boost::geometry::get<0>(points[i]);
                double py = boost::geometry::get<1>(points[i]);
                normalizedX = -1.0 + 2.0 * px / (double)gWindowWidth;
                normalizedY = 1.0 - 2.0 * py / (double)gWindowHeight;
                extract_points.push_back(glm::vec3(normalizedX, normalizedY, 0.0));
            }
            result_data.push_back(extract_points);

            // for (int i = 0; i < Px.size(); i++)
            // {
            //     std::cout << "Px[" << i << "] = " << Px[i] << ", Py[" << i << "] = " << Py[i] << std::endl;
            // }
        }
        computation_done = true;
    }
    // {
    //     typedef boost::geometry::model::d2::point_xy<double> point;
    //     typedef boost::geometry::model::polygon<point> polygon;
    //     polygon poly_truth;
    //     for (size_t i = 0; i < truth_data.size(); i++)
    //     {
    //         poly_truth.outer().push_back(point(truth_data_real[i].x, truth_data_real[i].y));
    //     }
    //     boost::geometry::correct(poly_truth);
    //     // std::cout << boost::geometry::wkt(poly_truth) << std::endl;
    //     std::string reason;
    //     bool first_valid = boost::geometry::is_valid(poly_truth, reason);
    //     std::cout << " - " << (!first_valid ? "invalid (" + reason + ")" : " truth data valid") << "\n";

    //     polygon poly_raw;
    //     for (size_t i = 0; i < raw_data.size(); i++)
    //     {
    //         poly_raw.outer().push_back(point(raw_data_real[i].x, raw_data_real[i].y));
    //     }
    //     boost::geometry::correct(poly_raw);
    //     std::deque<polygon> output_intersection;
    //     bool second_valid = boost::geometry::is_valid(poly_raw, reason);
    //     std::cout << " - " << (!second_valid ? "invalid (" + reason + ")" : " raw data valid") << "\n";
    //     boost::geometry::intersection(poly_raw, poly_truth, output_intersection);
    //     int k = 0;
    //     BOOST_FOREACH (polygon const &p, output_intersection)
    //     {
    //         std::cout << "INTERSECT " << k++ << ": " << boost::geometry::wkt(p) << std::endl;
    //         // polygon new_intersection_output = p;
    //         // std::vector<point> const &points = new_intersection_output.outer();
    //         // std::vector<double> Px;
    //         // std::vector<double> Py;

    //         // for (std::vector<point>::size_type i = 0; i < points.size(); ++i)
    //         // {
    //         //     Px.push_back(boost::geometry::get<0>(points[i]));
    //         //     Py.push_back(boost::geometry::get<1>(points[i]));
    //         // }

    //         // std::cout << "Size Px = " << Px.size() << std::endl;
    //         // std::cout << "Size Py = " << Py.size() << std::endl;

    //         // for (int i = 0; i < Px.size(); i++)
    //         // {
    //         //     std::cout << "Px[" << i << "] = " << Px[i] << ", Py[" << i << "] = " << Py[i] << std::endl;
    //         // }
    //     }
    // }
}
static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    // std::cout << xpos << " : " << ypos << std::endl;
    //setup ground truth
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!close_truth)
        {
            // std::cout << "GroundTruth\n";
            normalizedX = -1.0 + 2.0 * xpos / (double)gWindowWidth;
            normalizedY = 1.0 - 2.0 * ypos / (double)gWindowHeight;
            number_of_truth_points++;
            // std::cout << normalizedX << " : " << normalizedY << std::endl;

            truth_data.push_back(glm::vec3(normalizedX, normalizedY, 0));
            truth_data_real.push_back(glm::vec3(xpos, ypos, 0));
        }
        else
        {
            // std::cout << "Raw\n";
            normalizedX = -1.0 + 2.0 * xpos / (double)gWindowWidth;
            normalizedY = 1.0 - 2.0 * ypos / (double)gWindowHeight;
            number_of_probe_points++;
            // std::cout
            //     << normalizedX << " : " << normalizedY << std::endl;

            raw_data.push_back(glm::vec3(-1.0 + 2.0 * xpos / (double)gWindowWidth, 1.0 - 2.0 * ypos / (double)gWindowHeight, 0));
            raw_data_real.push_back(glm::vec3(xpos, ypos, 0));
        }
    }
    //start drawing
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && start_draw)
    // {
    //     std::cout << "Raw\n";
    //     normalizedX = -1.0 + 2.0 * xpos / (double)gWindowWidth;
    //     normalizedY = 1.0 - 2.0 * ypos / (double)gWindowHeight;
    //     number_of_probe_points++;
    //     std::cout
    //         << normalizedX << " : " << normalizedY << std::endl;

    //     raw_data.push_back(glm::vec3(-1.0 + 2.0 * xpos / (double)gWindowWidth, 1.0 - 2.0 * ypos / (double)gWindowHeight, 0));
    // }
}

void glfw_onFramebufferSize(GLFWwindow *window, int width, int height)
{
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, gWindowWidth, gWindowHeight);
}
std::string getOsName()
{
#ifdef _WIN32
    return "Windows 32-bit";
#elif _WIN64
    return "Windows 64-bit";
#elif __unix || __unix__
    return "Unix";
#elif __APPLE__ || __MACH__
    return "Mac";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#else
    return "Other";
#endif
}
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // glEnable(GL_CULL_FACE);

    // Activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},
            {xpos, ypos, 0.0, 1.0},
            {xpos + w, ypos, 1.0, 1.0},

            {xpos, ypos + h, 0.0, 0.0},
            {xpos + w, ypos, 1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}};
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        // glEnable(GL_CULL_FACE);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDisable(GL_CULL_FACE);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
