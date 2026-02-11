#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float aspect, float nearP, float farP)
    : m_fov(fov), m_aspect(aspect), m_near(nearP), m_far(farP)
{
}

void Camera::processInput(GLFWwindow* window, float dt)
{
    float speed = 60.0f * dt;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_yaw -= speed;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_yaw += speed;

    if (m_yaw > 90.0f)  m_yaw = 90.0f;
    if (m_yaw < -90.0f) m_yaw = -90.0f;
}

glm::mat4 Camera::getView() const
{
    float rad = glm::radians(m_yaw);

    glm::vec3 position;
    position.x = sin(rad) * m_distance;
    position.z = cos(rad) * m_distance;
    position.y = 3.0f;

    return glm::lookAt(position, m_target, { 0,1,0 });
}

glm::mat4 Camera::getProjection() const
{
    return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}
