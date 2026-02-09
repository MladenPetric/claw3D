#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovDeg, float aspect, float nearP, float farP)
{
    // Kamera stoji ispred scene i gleda u centar
    m_view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),   // pozicija
        glm::vec3(0.0f, 0.0f, 0.0f),   // target
        glm::vec3(0.0f, 1.0f, 0.0f)    // up
    );

    m_projection = glm::perspective(
        glm::radians(fovDeg),
        aspect,
        nearP,
        farP
    );
}

const glm::mat4& Camera::getView() const
{
    return m_view;
}

const glm::mat4& Camera::getProjection() const
{
    return m_projection;
}
