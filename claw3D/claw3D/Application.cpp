#include "Application.h"

#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Application::Application()
    : m_running(true)
{
}

Application::~Application()
{
    shutdown();
}

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
    if (!m_window.create(1280, 720, "Claw 3D"))
    {
        m_running = false;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    m_shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
    m_camera = new Camera(60.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    m_cube = new Mesh(cubeVertices, 36);

    /*initTriangle();*/
}

//void Application::initTriangle()
//{
//    float vertices[] = {
//         0.0f,  0.5f, 0.0f,
//        -0.5f, -0.5f, 0.0f,
//         0.5f, -0.5f, 0.0f
//    };
//
//    glGenVertexArrays(1, &m_VAO);
//    glGenBuffers(1, &m_VBO);
//
//    glBindVertexArray(m_VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(
//        0, 3, GL_FLOAT, GL_FALSE,
//        3 * sizeof(float), (void*)0
//    );
//    glEnableVertexAttribArray(0);
//
//    glBindVertexArray(0);
//}
void Application::drawScene()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp =
        m_camera->getProjection() *
        m_camera->getView() *
        model;

    m_shader->use();
    m_shader->setMat4("u_MVP", mvp);

    m_cube->draw();
}


void Application::update(float)
{
}

void Application::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();
}

void Application::shutdown()
{
    /*if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);*/

    delete m_shader;
    m_shader = nullptr;

    delete m_camera;
    m_camera = nullptr;

    delete m_cube;
    m_cube = nullptr;


    std::cout << "Application shutdown\n";
}
