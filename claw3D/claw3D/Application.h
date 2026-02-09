#pragma once

#include "Window.h"
#include "Shader.h"

class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    void init();
    void update(float dt);
    void render();
    void shutdown();

    bool m_running;
    // === PROZOR ===
    Window m_window;
    // === OpenGL test (trougao) ===
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    Shader* shader = nullptr;
};
