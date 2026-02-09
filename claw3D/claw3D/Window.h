#pragma once

struct GLFWwindow;

class Window
{
public:
    Window();
    ~Window();

    bool create(int width, int height, const char* title);
    void pollEvents();
    void swapBuffers();

    bool shouldClose() const;
    GLFWwindow* getHandle() const;

private:
    GLFWwindow* m_window;
};
