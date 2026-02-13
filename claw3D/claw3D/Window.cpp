#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window() : m_window(nullptr) {}

Window::~Window()
{
    if (m_window)
        glfwDestroyWindow(m_window);

    glfwTerminate();
}

bool Window::create(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    m_window = glfwCreateWindow(
        mode->width,
        mode->height,
        title,
        monitor,      // fullscreen
        nullptr
    );

    if (!m_window)
    {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glViewport(0, 0, mode->width, mode->height);

    glfwSwapInterval(0);

    return true;
}


void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

GLFWwindow* Window::getHandle() const
{
    return m_window;
}
