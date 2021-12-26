#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"

static const int _WIDTH = 1920;
static const int _HEIGHT = 1080;
static const char* _TITLE = "Game";

Shader* shaderProgram;

VAO* vao1;

GLuint scale;

GLuint texture;

float x = 0, y = 0;

void GLFWHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void CreateTriangle()
{
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f,      /*0.5f, 0.3f, 0.9f,*/     0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,       /*0.5f, 0.3f, 0.0f,*/     0.0f, 1.0f,
        0.5f, 0.5f, 0.0f,        /*0.5f, 0.0f, 0.9f,*/     1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,       /*0.0f, 0.3f, 0.9f,*/     1.0f, 0.0f
    };

    GLuint indices[] =
    {
        0, 2, 1,
        0, 2, 1
    };

    shaderProgram = new Shader("default.vert", "default.frag");

    vao1 = new VAO();

    vao1->Bind();

    VBO vbo1(vertices, sizeof(vertices));
    EBO ebo1(indices, sizeof(indices));

    vao1->LinkAttrib(vbo1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    //vao1->LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1->LinkAttrib(vbo1, 2, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1->Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    scale = glGetUniformLocation(shaderProgram->ID, "scale");

    int width, height, colChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("tex.png", &width, &height, &colChannel, 0);

    
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0 = glGetUniformLocation(shaderProgram->ID, "tex0");
    shaderProgram->Activate();
    glUniform1i(tex0, 0);
}

GLFWwindow* CreateWindow(int WIDTH, int HEIGHT, const char* TITLE)
{
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) return NULL;

    glfwMakeContextCurrent(window);

    return window;
}

void SetWindowColor(GLfloat r, GLfloat g, GLfloat b, GLFWwindow* window)
{
    gladLoadGL();

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

int main(void)
{
    glfwInit();

    GLFWHints();

    GLFWwindow* window = CreateWindow(_WIDTH, _HEIGHT, _TITLE);

    //SetWindowColor(0.3f, 0.4f, 0.2f, window);
    gladLoadGL();
    CreateTriangle();

    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, 1920, 1080);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram->Activate();
        glUniform1f(scale, .3f);
        glBindTexture(GL_TEXTURE_2D, texture);
        vao1->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao1->Delete();

    shaderProgram->Delete();

    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}