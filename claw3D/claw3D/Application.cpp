#include "Application.h"
#include <iostream>
#include <chrono>

#include <glad/glad.h>

Application::Application()
    : m_running(true)
{
}

Application::~Application()
{
    shutdown();
}

void Application::run()
{
    init();

    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (m_running && !m_window.shouldClose())
    {
        auto currentTime = clock::now();
        std::chrono::duration<float> delta = currentTime - lastTime;
        lastTime = currentTime;

        update(delta.count());
        render();

        m_window.swapBuffers();
        m_window.pollEvents();
    }
}

void Application::init()
{
    std::cout << "Application init\n";

    if (!m_window.create(1280, 720, "Claw 3D"))
    {
        m_running = false;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    // === SHADER ===
    shader = new Shader("shaders/basic.vert", "shaders/basic.frag");

    // === TROUGAO ===
    float verts[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);
}

void Application::update(float)
{
}

void Application::render()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::shutdown()
{
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);

    delete shader;
    shader = nullptr;

    std::cout << "Application shutdown\n";
}
