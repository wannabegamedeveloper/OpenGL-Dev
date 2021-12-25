#include <GLFW/glfw3.h>

static const int _WIDTH = 1920;
static const int _HEIGHT = 1080;

void GLFWHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* CreateWindow(int WIDTH, int HEIGHT)
{
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);
    if (window == NULL) return NULL;

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
        glfwPollEvents();

    return window;
}

int main(void)
{
    glfwInit();

    GLFWHints();

    GLFWwindow* window = CreateWindow(_WIDTH, _HEIGHT);

    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}