#include "Application.h"

#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "GameObject.h"

// === Shared cube geometry ===
static float cubeVertices[] = {
    // pos                 // normal
    // FRONT (0,0,1)
    -0.5f,-0.5f, 0.5f,   0,0,1,
     0.5f,-0.5f, 0.5f,   0,0,1,
     0.5f, 0.5f, 0.5f,   0,0,1,
     0.5f, 0.5f, 0.5f,   0,0,1,
    -0.5f, 0.5f, 0.5f,   0,0,1,
    -0.5f,-0.5f, 0.5f,   0,0,1,

    // BACK (0,0,-1)
    -0.5f,-0.5f,-0.5f,   0,0,-1,
    -0.5f, 0.5f,-0.5f,   0,0,-1,
     0.5f, 0.5f,-0.5f,   0,0,-1,
     0.5f, 0.5f,-0.5f,   0,0,-1,
     0.5f,-0.5f,-0.5f,   0,0,-1,
    -0.5f,-0.5f,-0.5f,   0,0,-1,

    // LEFT (-1,0,0)
    -0.5f, 0.5f, 0.5f,  -1,0,0,
    -0.5f, 0.5f,-0.5f,  -1,0,0,
    -0.5f,-0.5f,-0.5f,  -1,0,0,
    -0.5f,-0.5f,-0.5f,  -1,0,0,
    -0.5f,-0.5f, 0.5f,  -1,0,0,
    -0.5f, 0.5f, 0.5f,  -1,0,0,

    // RIGHT (1,0,0)
     0.5f, 0.5f, 0.5f,   1,0,0,
     0.5f,-0.5f,-0.5f,   1,0,0,
     0.5f, 0.5f,-0.5f,   1,0,0,
     0.5f,-0.5f,-0.5f,   1,0,0,
     0.5f, 0.5f, 0.5f,   1,0,0,
     0.5f,-0.5f, 0.5f,   1,0,0,

     // TOP (0,1,0)
     -0.5f, 0.5f,-0.5f,   0,1,0,
     -0.5f, 0.5f, 0.5f,   0,1,0,
      0.5f, 0.5f, 0.5f,   0,1,0,
      0.5f, 0.5f, 0.5f,   0,1,0,
      0.5f, 0.5f,-0.5f,   0,1,0,
     -0.5f, 0.5f,-0.5f,   0,1,0,

     // BOTTOM (0,-1,0)
     -0.5f,-0.5f,-0.5f,   0,-1,0,
      0.5f,-0.5f, 0.5f,   0,-1,0,
     -0.5f,-0.5f, 0.5f,   0,-1,0,
      0.5f,-0.5f, 0.5f,   0,-1,0,
     -0.5f,-0.5f,-0.5f,   0,-1,0,
      0.5f,-0.5f,-0.5f,   0,-1,0,
};

Application::Application() = default;

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

    m_cubeMesh = new Mesh(cubeVertices, 36);

    // BASE
    auto* base = m_scene->createObject(m_cubeMesh, "Base");
    base->transform.scale = { 3.0f, 1.0f, 3.0f };
    base->transform.position = { 0.0f, -1.0f, 0.0f };
    base->color = { 0.65f, 0.65f, 0.65f };

    // GLASS
    auto* glass = m_scene->createObject(m_cubeMesh, "Glass");
    glass->transparent = true;
    glass->transform.scale = { 3.2f, 3.0f, 3.2f };
    glass->transform.position = { 0.0f, 1.2f, 0.0f };
    glass->color = { 0.8f, 0.9f, 1.0f };

    // CLAW ROOT
    auto* clawRoot = m_scene->createObject(nullptr, "ClawRoot");
    clawRoot->transform.scale = { 1.5f, 1.5f, 1.5f };
    clawRoot->transform.position = { 0.0f, 2.5f, 0.0f };


    // VERTICAL ROD
    auto* verticalRod = m_scene->createObject(m_cubeMesh, "VerticalRod");
    verticalRod->transform.scale = { 0.1f, 1.0f, 0.1f };
    verticalRod->transform.position = { 0.0f, -0.5f, 0.0f };
    verticalRod->color = { 0.2f, 0.2f, 0.2f };
    clawRoot->addChild(verticalRod);

    // CLAW HEAD
    auto* clawHead = m_scene->createObject(m_cubeMesh, "ClawHead");
    clawHead->transform.scale = { 0.3f, 0.2f, 0.3f };
    clawHead->transform.position = { 0.0f, -1.0f, 0.0f };
    clawHead->color = { 0.8f, 0.2f, 0.2f };
    verticalRod->addChild(clawHead);

    // FINGERS
    for (int i = 0; i < 3; i++)
    {
        auto* finger = m_scene->createObject(m_cubeMesh, "Finger");
        finger->transform.scale = { 0.1f, 0.4f, 0.1f };
        finger->transform.position = {
            (i - 1) * 0.2f,
            -0.4f,
            0.2f
        };
        finger->color = { 0.9f, 0.9f, 0.9f };
        clawHead->addChild(finger);
    }


    m_clawRoot = clawRoot;


    for (int i = 0; i < 3; i++)
    {
        auto* toy = m_scene->createObject(m_cubeMesh, "Toy");
        toy->transform.scale = { 0.4f, 0.4f, 0.4f };
        toy->transform.position = {
            -0.6f + i * 0.6f,
            -0.6f,
            0.0f
        };
        toy->color = { 0.6f + 0.1f * i, 0.3f, 0.2f };
    }

    auto* dropZone = m_scene->createObject(m_cubeMesh, "DropZone");
    dropZone->transform.scale = { 0.8f, 0.1f, 0.4f };
    dropZone->transform.position = { 1.5f, -0.6f, 0.0f };
    dropZone->color = { 0.1f, 0.1f, 0.1f };

    auto* floor = m_scene->createObject(m_cubeMesh, "InnerFloor");
    floor->transform.scale = { 3.8f, 0.2f, 3.8f };
    floor->transform.position = { 0.0f, -0.8f, 0.0f };
    floor->color = { 0.3f, 0.3f, 0.3f };

    /*auto* backWall = m_scene->createObject(m_cubeMesh, "BackWall");
    backWall->transform.scale = { 3.8f, 3.8f, 0.1f };
    backWall->transform.position = { 0.0f, 1.5f, -1.9f };
    backWall->color = { 0.4f, 0.8f, 0.5f };*/


}

void Application::update(float dt)
{
    m_camera->processInput(m_window.getHandle(), dt);
    m_scene->update(dt);

    float move = m_clawSpeed * dt;

    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_A) == GLFW_PRESS)
        m_clawRoot->transform.position.x -= move;

    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_D) == GLFW_PRESS)
        m_clawRoot->transform.position.x += move;

    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_W) == GLFW_PRESS)
        m_clawRoot->transform.position.z -= move;

    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_S) == GLFW_PRESS)
        m_clawRoot->transform.position.z += move;

    // clamp unutar kutije
    m_clawRoot->transform.position.x = glm::clamp(
        m_clawRoot->transform.position.x, -1.0f, 1.0f);

    m_clawRoot->transform.position.z = glm::clamp(
        m_clawRoot->transform.position.z, -1.0f, 1.0f);

    // drop logika
    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_dropping = true;

    if (m_dropping)
    {
        m_clawRoot->transform.position.y -= m_dropSpeed * dt;

        if (m_clawRoot->transform.position.y <= 0.3f)
            m_dropping = false;
    }
    else
    {
        if (m_clawRoot->transform.position.y < 1.5f)
            m_clawRoot->transform.position.y += m_dropSpeed * dt;
    }

}

void Application::render()
{
    glClearColor(0.7f, 0.85f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    // svetlo (minimalno za specifikaciju)
    m_shader->use();
    m_shader->setVec3("u_LightPos", m_lightPos);
    m_shader->setVec3("u_LightColor", m_lightColor);
    m_shader->setVec3("u_ViewPos", glm::vec3(0.0f, 0.0f, 0.0f)); // ako Camera nema poziciju, privremeno
    m_shader->setFloat("u_Shininess", 32.0f);

    
    // prvo crtamo sve cvrste objekte
    m_shader->use();
    m_shader->setFloat("u_Alpha", 1.0f);


    m_scene->draw(*m_shader, *m_camera);
}

void Application::shutdown()
{
    delete m_scene;   m_scene = nullptr;
    delete m_camera;  m_camera = nullptr;
    delete m_shader;  m_shader = nullptr;

    delete m_cubeMesh; m_cubeMesh = nullptr;
}
