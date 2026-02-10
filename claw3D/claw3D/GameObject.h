#pragma once

#include <glm/glm.hpp>

class Mesh;
class Shader;
class Camera;

class GameObject
{
public:
    GameObject(Mesh* mesh);

    void setPosition(const glm::vec3& pos);
    void setRotation(const glm::vec3& rot);
    void setScale(const glm::vec3& scale);

    void draw(const Shader& shader, const Camera& camera) const;

private:
    Mesh* m_mesh;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};
