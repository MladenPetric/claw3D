#include "Application.h"
#include <iostream>
#include <chrono>

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

    while (m_running)
    {
        auto currentTime = clock::now();
        std::chrono::duration<float> delta = currentTime - lastTime;
        lastTime = currentTime;

        update(delta.count());
        render();
    }
}

void Application::init()
{
    std::cout << "Application init\n";
    // OVDE IDE KASNIJE:
    // - window
    // - OpenGL init
    // - kamera
}

void Application::update(float dt)
{
    // OVDE IDE:
    // - input
    // - logika igre
}

void Application::render()
{
    // OVDE IDE:
    // - glClear
    // - crtanje scene
}

void Application::shutdown()
{
    std::cout << "Application shutdown\n";
}
