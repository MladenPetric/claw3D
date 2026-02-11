#pragma once
#include "Window.h"
#include <vector>
#include "GameObject.h"
#include <glm/glm.hpp>

class Shader;
class Camera;
class Mesh;
class Scene;

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

private:
    bool m_running = true;
    Window m_window;

    Shader* m_shader = nullptr;
    Camera* m_camera = nullptr;
    Scene* m_scene = nullptr;

    Mesh* m_cubeMesh = nullptr; // da ne curi memorija

    glm::vec3 m_lightPos{ 2.5f, 3.0f, 2.0f };
    glm::vec3 m_lightColor{ 1.0f, 1.0f, 1.0f };

    GameObject* m_clawRoot = nullptr;
    float m_clawSpeed = 2.0f;
    float m_dropSpeed = 2.0f;
    bool m_dropping = false;

    Shader* m_watermarkShader = nullptr;

    unsigned int m_watermarkVAO = 0;
    unsigned int m_watermarkVBO = 0;
    unsigned int m_watermarkTexture = 0;

};
