#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(float fov, float aspect, float nearP, float farP);

    void processInput(GLFWwindow* window, float dt);

    glm::mat4 getView() const;
    glm::mat4 getProjection() const;

private:
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    float m_yaw = 0.0f;
    float m_distance = 8.0f;

    glm::vec3 m_target = { 0.0f, 0.5f, 0.0f };
};
