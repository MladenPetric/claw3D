#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(float fovDeg, float aspect, float nearP, float farP);

    const glm::mat4& getView() const;
    const glm::mat4& getProjection() const;

private:
    glm::mat4 m_view;
    glm::mat4 m_projection;
};
