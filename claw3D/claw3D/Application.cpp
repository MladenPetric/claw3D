#include "Application.h"

#include <iostream>
#include <chrono>

//#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

// === Shared cube geometry ===
static float cubeVertices[] = {
    // front
    -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,

     // back
     -0.5f,-0.5f,-0.5f, -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
      0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f,

      // left
      -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
      -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,

      // right
       0.5f, 0.5f, 0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
       0.5f,-0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f,-0.5f, 0.5f,

       // top
       -0.5f, 0.5f,-0.5f, -0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f,

        // bottom
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f
};

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
        auto now = clock::now();
        std::chrono::duration<float> dt = now - lastTime;
        lastTime = now;

        update(dt.count());
        render();

        m_window.swapBuffers();
        m_window.pollEvents();
    }
}

void Application::init()
{
    if (!m_window.create(1280, 720, "Claw 3D"))
    {
        m_running = false;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    m_shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
    m_camera = new Camera(60.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

    m_scene = new Scene();

    Mesh* cubeMesh = new Mesh(cubeVertices, 36);

    // BASE
    GameObject base(cubeMesh);
    base.transform.scale = { 2.0f, 1.0f, 2.0f };
    base.transform.position = { 0.0f, -0.5f, 0.0f };
    m_scene->addObject(base);

    // GLASS
    GameObject glass(cubeMesh);
    glass.transform.scale = { 2.0f, 2.0f, 2.0f };
    glass.transform.position = { 0.0f, 0.5f, 0.0f };
    m_scene->addObject(glass);

    // CLAW
    GameObject claw(cubeMesh);
    claw.transform.scale = { 0.2f, 1.0f, 0.2f };
    claw.transform.position = { 0.0f, 1.5f, 0.0f };
    m_scene->addObject(claw);
}

void Application::update(float dt)
{
    m_camera->processInput(m_window.getHandle(), dt);
    m_scene->update(dt);
}



void Application::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_scene->draw(*m_shader, *m_camera);
}

void Application::shutdown()
{
    delete m_scene;
    m_scene = nullptr;

    delete m_camera;
    m_camera = nullptr;

    delete m_shader;
    m_shader = nullptr;

    std::cout << "Application shutdown\n";
}
