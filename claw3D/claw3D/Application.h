#pragma once
#include "Window.h"
#include <vector>
#include "GameObject.h"
#include <glm/glm.hpp>
class Shader;
class Camera;
class Mesh;
class Scene;

struct GLFWcursor;


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

    Mesh* m_cubeMesh = nullptr;

    Mesh* m_sphereMesh = nullptr;
    GameObject* m_lamp = nullptr;

    enum class LampMode { Off, Blue, WinBlink };
    LampMode m_lampMode = LampMode::Off;
    float m_lampBlinkTimer = 0.0f;
    bool m_lampBlinkGreen = true;

    glm::vec3 m_lightPos{ 0.0f, 3.0f, 0.0f };
    glm::vec3 m_lightColor{ 0.0f, 0.0f, 0.0f };

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

    GLFWcursor* m_coinCursor = nullptr;
    GLFWcursor* m_leverCursor = nullptr;

    bool m_machineOn = false;   // stanje automata

    // ===== GAME STATE =====
    enum class GameState { Off, Playing, Dropping, Returning, Carrying, ToyFalling, PrizeBlink };
    GameState m_state = GameState::Off;

    GameObject* m_coinSlotObj = nullptr;

    GameObject* m_teddyObj = nullptr;
    GameObject* m_sheepObj = nullptr;

    std::vector<GameObject*> m_toys;
    std::vector<GameObject*> m_fingers;

    GameObject* m_grabbedToy = nullptr;   // trenutno u kandži
    GameObject* m_prizeToy = nullptr;     // osvojena 

    glm::vec3 m_toyVelocity{ 0.0f };
    bool m_spaceDown = false;
    bool m_lmbDown = false;

    // ===== TUNING =====
    float m_clawStartY = 2.5f;
    float m_clawMinY = -0.25f;



    float m_floorY = -0.6f;               // "dno kutije" za igračke
    float m_grabRadius = 0.35f;           // hvatanje igračke 

    glm::vec3 m_holeCenter{ 0.0f, -0.6f, 1.0f };
    glm::vec2 m_holeHalfSize{ 0.6f, 0.5f };

    // pola širine/dužine rupe (x,z)

    glm::vec3 m_prizePos{ 0.0f, -2.2f, 2.2f };    // gde se vidi osvojena igračka 


    void setClawOpen(bool open);
	void updateCursor();
    bool m_clawOpen = true;

    glm::vec3 m_grabOffset{ 0.0f, -0.4f, 0.0f };




};
