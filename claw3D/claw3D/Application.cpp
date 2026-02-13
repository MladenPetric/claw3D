#include "Application.h"

#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>

#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "GameObject.h"
#include "stb_image.h"
#include "Util.h"
#include <thread>

#include <vector>


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


static bool isFrontPlayable(const glm::vec3& camPos, bool closeView)
{

    if (!closeView) return false;
    return camPos.z > 3.0f; 
}

static bool pointInHoleXZ(const glm::vec3& p, const glm::vec3& holeCenter, const glm::vec2& halfSize)
{
    return (std::abs(p.x - holeCenter.x) <= halfSize.x) &&
        (std::abs(p.z - holeCenter.z) <= halfSize.y);
}


static std::vector<float> buildSphereVertices(int stacks, int slices)
{
    std::vector<float> v;
    v.reserve(stacks * slices * 6 * 6);

    auto push = [&](const glm::vec3& p)
        {
            glm::vec3 n = glm::normalize(p);
            v.push_back(p.x); v.push_back(p.y); v.push_back(p.z);
            v.push_back(n.x); v.push_back(n.y); v.push_back(n.z);
        };

    for (int i = 0; i < stacks; ++i)
    {
        float v0 = (float)i / (float)stacks;
        float v1 = (float)(i + 1) / (float)stacks;
        float phi0 = glm::pi<float>() * (v0 - 0.5f);
        float phi1 = glm::pi<float>() * (v1 - 0.5f);

        for (int j = 0; j < slices; ++j)
        {
            float u0 = (float)j / (float)slices;
            float u1 = (float)(j + 1) / (float)slices;
            float th0 = glm::two_pi<float>() * u0;
            float th1 = glm::two_pi<float>() * u1;

            glm::vec3 p00 = { cos(phi0) * cos(th0), sin(phi0), cos(phi0) * sin(th0) };
            glm::vec3 p10 = { cos(phi1) * cos(th0), sin(phi1), cos(phi1) * sin(th0) };
            glm::vec3 p01 = { cos(phi0) * cos(th1), sin(phi0), cos(phi0) * sin(th1) };
            glm::vec3 p11 = { cos(phi1) * cos(th1), sin(phi1), cos(phi1) * sin(th1) };

            push(p00); push(p10); push(p11);
            push(p00); push(p11); push(p01);
        }
    }

    return v;
}

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

    const float targetFrameTime = 1.0f / 75.0f;

    while (m_running && !m_window.shouldClose())
    {
        auto frameStart = clock::now();
        std::chrono::duration<float> dt = frameStart - lastTime;
        lastTime = frameStart;

        update(dt.count());
        render();

        m_window.swapBuffers();
        m_window.pollEvents();

        // FRAME LIMITER
        auto frameEnd = clock::now();
        std::chrono::duration<float> frameTime = frameEnd - frameStart;

        if (frameTime.count() < targetFrameTime)
        {
            std::this_thread::sleep_for(
                std::chrono::duration<float>(targetFrameTime - frameTime.count())
            );
        }
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

    // ===== TEDDY OBJ + TEXTURE =====
    Mesh* teddyMesh = new Mesh(std::string("models/Teddy.obj"));
    m_teddyObj = m_scene->createObject(teddyMesh, "Teddy");
    m_teddyObj->transform.scale = { 0.15f, 0.15f, 0.15f };
    m_teddyObj->transform.position = { 0.0f, -0.4f, 0.0f };
    m_teddyObj->texture = loadImageToTexture("models/Teddy.png");
    m_teddyObj->useTexture = true;
    m_toys.push_back(m_teddyObj);

    m_coinCursor = loadImageToCursor("Resources/handCursor.png");
    m_leverCursor = loadImageToCursor("Resources/lever.png");

    // početno stanje – automat isključen → žeton
    glfwSetCursor(m_window.getHandle(), m_coinCursor);
    m_machineOn = false;


    // ===== SHEEP OBJ + TEXTURE =====
    Mesh* sheepMesh = new Mesh(std::string("models/Sheep.obj"));
    m_sheepObj = m_scene->createObject(sheepMesh, "Sheep");
    m_sheepObj->transform.scale = { 0.7f, 0.7f, 0.7f };
    m_sheepObj->transform.position = { 1.0f, -0.5f, 0.8f };
    m_sheepObj->texture = loadImageToTexture("models/Sheep.png");
    m_sheepObj->useTexture = true;
    m_toys.push_back(m_sheepObj);



    // Sphere mesh (lamp)
    {
        auto sphere = buildSphereVertices(18, 24);
        m_sphereMesh = new Mesh(sphere.data(), (unsigned int)(sphere.size() / 6));
    }

    // =====================
// 1. GLAVNA MASIVNA BAZA
// =====================
    float baseWidth = 4.0f;
    float baseHeight = 1.8f;
    float baseDepth = 4.5f;

    float holeWidth = 1.2f;
    float holeHeight = 0.9f;
    float wallThickness = 0.3f;

    // LEVA STRANA
    auto* baseLeft = m_scene->createObject(m_cubeMesh, "BaseLeft");
    baseLeft->transform.scale = { (baseWidth - holeWidth) / 2.0f, baseHeight, baseDepth };
    baseLeft->transform.position = { -((holeWidth + baseWidth) / 4.0f), -1.5f, 0.0f };
    baseLeft->color = { 0.2f, 0.2f, 0.2f };

    // DESNA STRANA
    auto* baseRight = m_scene->createObject(m_cubeMesh, "BaseRight");
    baseRight->transform.scale = { (baseWidth - holeWidth) / 2.0f, baseHeight, baseDepth };
    baseRight->transform.position = { ((holeWidth + baseWidth) / 4.0f), -1.5f, 0.0f };
    baseRight->color = { 0.2f, 0.2f, 0.2f };

    float upperPartHeight = baseHeight - holeHeight;
    float upperCenterY = -1.5f + holeHeight / 2.0f;

    // LEVI GORNJI DEO (pod u staklu - levo od rupe)
    auto* upperLeft = m_scene->createObject(m_cubeMesh, "UpperLeft");
    upperLeft->transform.scale = {
        (baseWidth - holeWidth) / 2.0f,
        upperPartHeight,
        baseDepth
    };
    upperLeft->transform.position = {
        -((holeWidth + baseWidth) / 4.0f),
        upperCenterY,
        0.0f
    };
    upperLeft->color = { 0.2f, 0.2f, 0.2f };

    // DESNI GORNJI DEO
    auto* upperRight = m_scene->createObject(m_cubeMesh, "UpperRight");
    upperRight->transform.scale = {
        (baseWidth - holeWidth) / 2.0f,
        upperPartHeight,
        baseDepth
    };
    upperRight->transform.position = {
        ((holeWidth + baseWidth) / 4.0f),
        upperCenterY,
        0.0f
    };
    upperRight->color = { 0.2f, 0.2f, 0.2f };

    // ZADNJI GORNJI DEO (iza rupe)
    auto* upperBack = m_scene->createObject(m_cubeMesh, "UpperBack");
    upperBack->transform.scale = {
        holeWidth,
        upperPartHeight,
        (baseDepth - holeWidth)
    };
    upperBack->transform.position = {
        0.0f,
        upperCenterY,
        -((holeWidth) / 2.0f)
    };
    upperBack->color = { 0.2f, 0.2f, 0.2f };


    // DONJA IVICA RUPE
    auto* baseBottomEdge = m_scene->createObject(m_cubeMesh, "BaseBottomEdge");
    baseBottomEdge->transform.scale = { holeWidth, holeHeight / 5.0f, baseDepth };
    baseBottomEdge->transform.position = { 0.0f, -1.85f - holeHeight / 2.0f, 0.0f };
    baseBottomEdge->color = { 0.2f, 0.2f, 0.2f };

    // =====================
// UNUTRAŠNJA PREGRADA (DA SE NE VIDI SPOJ RUPE)
// =====================

    float partitionThickness = 0.05f;
    float partitionHeight = baseHeight * 0.3f;   // skoro cela visina baze

    auto* innerPartition = m_scene->createObject(m_cubeMesh, "InnerPartition");
    innerPartition->transform.scale = { holeWidth, partitionHeight, partitionThickness };

    // malo uvučeno od prednje strane baze
    innerPartition->transform.position = {
        0.0f,
        -0.85f,
        baseDepth / 2.0f
    };

    innerPartition->color = { 0.15f, 0.15f, 0.15f };


    // GLASS
    auto* glass = m_scene->createObject(m_cubeMesh, "Glass");
    glass->transparent = true;
    glass->transform.scale = { 4.0f, 3.0f, 4.5f };
    glass->transform.position = { 0.0f, 0.9f, 0.0f };
    glass->color = { 0.8f, 0.9f, 1.0f };

    // LAMP (iznad stakla)
    m_lamp = m_scene->createObject(m_sphereMesh, "Lamp");
    m_lamp->transform.scale = { 0.25f, 0.25f, 0.25f };
    m_lamp->transform.position = { 0.0f, 2.85f, 0.0f };
    m_lamp->color = { 0.0f, 0.0f, 0.0f };

    // CLAW ROOT
    auto* clawRoot = m_scene->createObject(nullptr, "ClawRoot");
    clawRoot->transform.scale = { 1.5f, 1.5f, 1.5f };
    clawRoot->transform.position = { 0.0f, 2.5f, 0.0f };


    // VERTICAL ROD
    auto* verticalRod = m_scene->createObject(m_cubeMesh, "VerticalRod");
    verticalRod->transform.scale = { 0.1f, 1.0f, 0.1f };
    verticalRod->transform.position = { 0.0f, -0.6f, 0.0f };
    verticalRod->color = { 0.2f, 0.2f, 0.2f };
    clawRoot->addChild(verticalRod);

    auto* clawHead = m_scene->createObject(m_cubeMesh, "ClawHead");
    clawHead->transform.scale = { 0.2f, 0.2f, 0.2f };
    clawHead->transform.position = { 0.0f, -0.6f, 0.0f };
    clawHead->color = { 0.6f, 0.6f, 0.6f };
    verticalRod->addChild(clawHead);


    // FINGERS
    float fingerLength = 1.5f;
    float fingerThickness = 1.0f;
    float radius = 0.35f;

    for (int i = 0; i < 3; i++)
    {
        float angle = glm::radians(120.0f * i);

        auto* finger = m_scene->createObject(m_cubeMesh, "Finger");

        finger->transform.scale = {
            fingerThickness,
            fingerLength,
            fingerThickness
        };

        finger->transform.position = {
            cos(angle) * radius,
            -0.4f,
            sin(angle) * radius
        };

        finger->transform.rotation.x = -30.0f;   // nagib ka unutra
        finger->transform.rotation.y = glm::degrees(angle);

        finger->color = { 0.8f, 0.8f, 0.8f };

        clawHead->addChild(finger);
        m_fingers.push_back(finger);

    }


    m_clawRoot = clawRoot;

    // =====================
    // 4. RUPA ZA ŽETON (NA PREDNJEM PANELU)
    // =====================
    auto* coinSlot = m_scene->createObject(m_cubeMesh, "CoinSlot");
    coinSlot->transform.scale = { 0.7f, 0.12f, 0.05f };
    coinSlot->transform.position = { 1.0f, -1.0f, 2.25f };
    coinSlot->color = { 0.8f, 0.1f, 0.1f };

    m_coinSlotObj = coinSlot;


    // =====================
// 5. POLUGA (IZLAZI IZ PREDNJE STRANE BAZE)
// =====================

// pozicija prednje strane baze
    float frontZ = baseDepth / 2.0f;

    auto* leverRod = m_scene->createObject(m_cubeMesh, "LeverRod");
    leverRod->transform.scale = { 0.12f, 0.12f, 0.6f };   // izdužena po Z
    leverRod->transform.position = {
        1.6f,           // desna strana
        -1.2f,          // visina na bazi
        frontZ + 0.3f   // malo ispred baze
    };
    leverRod->color = { 0.15f, 0.15f, 0.15f };


    auto* leverHead = m_scene->createObject(m_cubeMesh, "LeverHead");
    leverHead->transform.scale = { 0.35f, 0.35f, 0.35f };
    leverHead->transform.position = {
        1.6f,
        -1.2f,
        frontZ + 0.65f  
    };
    leverHead->color = { 0.9f, 0.1f, 0.1f };



    // ===== WATERMARK =====
    m_watermarkShader = new Shader(
        "shaders/watermark.vert",
        "shaders/watermark.frag"
    );

    float watermarkVerts[] = {
        // X, Y, U, V
        -0.95f,  0.95f,  0.0f, 1.0f,
        -0.95f,  0.80f,  0.0f, 0.0f,
        -0.60f,  0.80f,  1.0f, 0.0f,
        -0.60f,  0.95f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_watermarkVAO);
    glGenBuffers(1, &m_watermarkVBO);

    glBindVertexArray(m_watermarkVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_watermarkVBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(watermarkVerts),
        watermarkVerts,
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float)));

    glBindVertexArray(0);

    stbi_set_flip_vertically_on_load(true);
    m_watermarkTexture = loadImageToTexture("Resources/Watermark.png");

    m_watermarkShader->use();
    m_watermarkShader->setInt("uTex", 0);


}

void Application::update(float dt)
{
    m_camera->processInput(m_window.getHandle(), dt);
    m_scene->update(dt);

    static bool lmbDown = false;
    static bool enterDown = false;
    static bool backDown = false;

    static bool lmbPressed = false;
// ===== TOGGLE POGLED=====
    if (glfwGetMouseButton(m_window.getHandle(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (!m_mousePressed)
        {
            m_mousePressed = true;
            m_closeView = !m_closeView;

            if (m_closeView)
                m_camera->setDistance(7.5f);
            else
                m_camera->setDistance(12.0f);
        }
    }
    else
    {
        m_mousePressed = false;
    }


    float move = m_clawSpeed * dt;

    // gasenje
    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window.getHandle(), true);
    }

    // ===== INPUT EDGES =====
    bool lmbNow = glfwGetMouseButton(m_window.getHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool lmbClick = lmbNow && !m_lmbDown;
    m_lmbDown = lmbNow;

    bool spaceNow = glfwGetKey(m_window.getHandle(), GLFW_KEY_SPACE) == GLFW_PRESS;
    bool spacePress = spaceNow && !m_spaceDown;
    m_spaceDown = spaceNow;

    // ===== VIEW GATE =====
    bool canPlayNow = isFrontPlayable(m_camera->getPosition(), m_closeView);

   // ===== OFF -> klik coin slot =====
    if (m_state == GameState::Off)
    {
        m_lampMode = LampMode::Off;

        if (lmbClick && canPlayNow)
        {
            m_state = GameState::Playing;
            m_machineOn = true;

            m_lampMode = LampMode::Blue;

            m_clawRoot->transform.position.y = m_clawStartY;
            m_dropping = false;
            m_returning = false;

            m_grabbedToy = nullptr;
            m_prizeToy = nullptr;

            setClawOpen(true);
        }

        updateCursor();
        return;
    }

    // ===== PRIZE BLINK -> klik na osvojenu igračku da nestane =====
    if (m_state == GameState::PrizeBlink)
    {
        m_lampMode = LampMode::WinBlink;

        if (lmbClick && canPlayNow && m_prizeToy)
        {
            // “nestane”: skloni je ispod scene i deaktiviraj
            m_prizeToy->active = false;

            m_prizeToy = nullptr;
            m_grabbedToy = nullptr;
            m_machineOn = false;

            m_lampMode = LampMode::Off;
            m_state = GameState::Off;
            m_machineOn = false;
            setClawOpen(true);

            return;
        }

        // u prize stanju nema kontrole kandže
        return;
    }

    /*float move = m_clawSpeed * dt;*/

    if (canPlayNow && (m_state == GameState::Playing || m_state == GameState::Carrying))
    {

        if (glfwGetKey(m_window.getHandle(), GLFW_KEY_A) == GLFW_PRESS)
            m_clawRoot->transform.position.x -= move;

        if (glfwGetKey(m_window.getHandle(), GLFW_KEY_D) == GLFW_PRESS)
            m_clawRoot->transform.position.x += move;

        if (glfwGetKey(m_window.getHandle(), GLFW_KEY_W) == GLFW_PRESS)
            m_clawRoot->transform.position.z -= move;

        if (glfwGetKey(m_window.getHandle(), GLFW_KEY_S) == GLFW_PRESS)
            m_clawRoot->transform.position.z += move;

        m_clawRoot->transform.position.x = glm::clamp(m_clawRoot->transform.position.x, -1.5f, 1.5f);
        m_clawRoot->transform.position.z = glm::clamp(m_clawRoot->transform.position.z, -1.8f, 1.8f);
        if (m_state == GameState::Carrying && m_grabbedToy)
        {
            m_grabbedToy->transform.position =
                m_clawRoot->transform.position + m_grabOffset;
        }
    }

    // ===== SPACE BEHAVIOR =====
    if (spacePress && canPlayNow)
    {
        // ako nosi igračku - ispusti
        if (m_state == GameState::Carrying && m_grabbedToy)
        {
            m_state = GameState::ToyFalling;
            m_toyVelocity = glm::vec3(0.0f); // start padanja
            setClawOpen(true);

        }
        // ako ne nosi i nije već u drop animaciji - kreni da spušta
        else if (m_state == GameState::Playing)
        {
            m_state = GameState::Dropping;
            setClawOpen(true);   // prvo otvorena
        }
    }

    glm::vec3 clawPos = m_clawRoot->transform.position;

    // ===== DROPPING =====
    if (m_state == GameState::Dropping)
    {
        // SPUŠTAJ KANDZU
        m_clawRoot->transform.position.y -= m_dropSpeed * dt;

        glm::vec3 clawPos = m_clawRoot->transform.position;

        for (GameObject* toy : m_toys)
        {
            if (!toy || !toy->active) continue;

            glm::vec3 tp = toy->transform.position;

            float dx = tp.x - clawPos.x;
            float dz = tp.z - clawPos.z;
            float distXZ = sqrt(dx * dx + dz * dz);

            if (distXZ <= m_grabRadius &&
                std::abs(clawPos.y - tp.y) <= 0.25f)
            {
                m_grabbedToy = toy;
                m_state = GameState::Returning;
                setClawOpen(false);
                break;
            }
        }

        // DOTAKLA DNO
        if (m_clawRoot->transform.position.y <= m_clawMinY)
        {
            m_clawRoot->transform.position.y = m_clawMinY;
            m_state = GameState::Returning;
        }
    }


    // ===== RETURNING =====
    if (m_state == GameState::Returning)
    {
        m_clawRoot->transform.position.y += m_dropSpeed * dt;

        // ako nosi igračku, neka ide sa kandzom
        if (m_grabbedToy)
        {
            m_grabbedToy->transform.position =
                m_clawRoot->transform.position + m_grabOffset;
        }


        if (m_clawRoot->transform.position.y >= m_clawStartY)
        {
            m_clawRoot->transform.position.y = m_clawStartY;

            if (m_grabbedToy)
                m_state = GameState::Carrying;
            else
                m_state = GameState::Playing;
        }
    }

    // ===== PADANNJE IGRACKE =====
    if (m_state == GameState::ToyFalling && m_grabbedToy)
    {
        m_toyVelocity.y -= 9.8f * dt;

        m_grabbedToy->transform.position += m_toyVelocity * dt;

        glm::vec3 p = m_grabbedToy->transform.position;

        if (pointInHoleXZ(p, m_holeCenter, m_holeHalfSize) && p.y < (m_floorY - 0.1f))
        {
            m_grabbedToy->transform.position = m_prizePos;

            m_prizeToy = m_grabbedToy;
            m_grabbedToy = nullptr;

            m_lampMode = LampMode::WinBlink;
            m_lampBlinkTimer = 0.0f;
            m_lampBlinkGreen = true;

            m_state = GameState::PrizeBlink;
        }
        else if (p.y <= m_floorY)
        {
            // udar u dno
            p.y = m_floorY;
            m_grabbedToy->transform.position = p;

            m_toyVelocity = glm::vec3(0.0f);
            m_state = GameState::Playing;
            m_grabbedToy = nullptr;
        }
    }

    
    // DEPTH toggle
    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_1) == GLFW_PRESS)
    {
        if (!m_dPressed)
        {
            m_dPressed = true;
            m_depthEnabled = !m_depthEnabled;

            if (m_depthEnabled)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);
        }
    }
    else
    {
        m_dPressed = false;
    }

    // CULL toggle
    if (glfwGetKey(m_window.getHandle(), GLFW_KEY_2) == GLFW_PRESS)
    {
        if (!m_cPressed)
        {
            m_cPressed = true;
            m_cullEnabled = !m_cullEnabled;

            if (m_cullEnabled)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        }
    }
    else
    {
        m_cPressed = false;
    }
    updateCursor();

}
void Application::setClawOpen(bool open)
{
    m_clawOpen = open;

    float rot = open ? -40.0f : 10.0f;



    for (GameObject* finger : m_fingers)
    {
        finger->transform.rotation.x = rot;
    }
}

void Application::updateCursor()
{
    if (m_state == GameState::Off) glfwSetCursor(m_window.getHandle(), m_coinCursor);
    else                           glfwSetCursor(m_window.getHandle(), m_leverCursor);
}

void Application::render()
{
    glClearColor(0.7f, 0.85f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    // ===== LAMP = svetlo na sceni =====
    glm::vec3 lampColor{ 0.0f };
    switch (m_lampMode)
    {
    case LampMode::Off:
        lampColor = { 1.0f, 1.0f, 1.0f };
        break;
    case LampMode::Blue:
        lampColor = { 0.1f, 0.35f, 1.0f };
        break;
    case LampMode::WinBlink:
        lampColor = m_lampBlinkGreen ? glm::vec3(0.1f, 1.0f, 0.2f) : glm::vec3(1.0f, 0.15f, 0.15f);
        break;
    }

    if (m_lamp)
    {
        m_lamp->color = lampColor;
        m_lightPos = m_lamp->transform.position;
    }
    m_lightColor = lampColor;

    m_shader->use();
    m_shader->setVec3("u_LightPos", m_lightPos);
    m_shader->setVec3("u_LightColor", m_lightColor);
    m_shader->setVec3("u_AmbientColor", m_ambientColor);
    m_shader->setFloat("u_AmbientStrength", m_ambientStrength);
    m_shader->setVec3("u_ViewPos", m_camera->getPosition());
    m_shader->setFloat("u_Shininess", 32.0f);


    m_shader->use();
    m_shader->setFloat("u_Alpha", 1.0f);


    m_scene->draw(*m_shader, *m_camera);


    // ===== DRAW WATERMARK =====
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_watermarkShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_watermarkTexture);

    glBindVertexArray(m_watermarkVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

}

void Application::shutdown()
{
    delete m_scene;   m_scene = nullptr;
    delete m_camera;  m_camera = nullptr;
    delete m_shader;  m_shader = nullptr;

    delete m_cubeMesh; m_cubeMesh = nullptr;

    delete m_sphereMesh; m_sphereMesh = nullptr;

    delete m_watermarkShader;
    m_watermarkShader = nullptr;

    if (m_watermarkVBO) glDeleteBuffers(1, &m_watermarkVBO);
    if (m_watermarkVAO) glDeleteVertexArrays(1, &m_watermarkVAO);

    if (m_coinCursor)  glfwDestroyCursor(m_coinCursor);
    if (m_leverCursor) glfwDestroyCursor(m_leverCursor);

}
