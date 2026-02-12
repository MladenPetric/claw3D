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

    Mesh* m_sphereMesh = nullptr;
    GameObject* m_lamp = nullptr;

    enum class LampMode { Off, Blue, WinBlink };
    LampMode m_lampMode = LampMode::Off;
    float m_lampBlinkTimer = 0.0f;
    bool m_lampBlinkGreen = true;

    // primarno svetlo dolazi iz lampice (u render() se puni iz m_lamp)
    glm::vec3 m_lightPos{ 0.0f, 3.0f, 0.0f };
    glm::vec3 m_lightColor{ 0.0f, 0.0f, 0.0f };

    // slab ambient, nezavisan od lampice
    glm::vec3 m_ambientColor{ 1.0f, 1.0f, 1.0f };
    float m_ambientStrength = 0.20f;

    GameObject* m_clawRoot = nullptr;
    float m_clawSpeed = 2.0f;
    float m_dropSpeed = 2.0f;
    bool m_dropping = false;
    bool m_returning = false;

    Shader* m_watermarkShader = nullptr;

    unsigned int m_watermarkVAO = 0;
    unsigned int m_watermarkVBO = 0;
    unsigned int m_watermarkTexture = 0;

    bool m_closeView = false;
    bool m_mousePressed = false;


    bool m_depthEnabled = true;
    bool m_cullEnabled = false;
    bool m_dPressed = false;
    bool m_cPressed = false;



};
