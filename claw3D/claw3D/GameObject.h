#pragma once
#include "Transform.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh;
class Shader;
class Camera;

class GameObject
{
public:
    explicit GameObject(Mesh* mesh = nullptr, std::string name = "");

    void draw(const Shader& shader, const Camera& camera) const;

    void setParent(GameObject* newParent);
    void addChild(GameObject* child);

public:
    std::string name;
    bool active = true;
    bool transparent = false;

    Transform transform;
    glm::vec3 color{ 1.0f, 0.0f, 0.0f };

private:
    Mesh* m_mesh = nullptr;

    GameObject* m_parent = nullptr;
    std::vector<GameObject*> m_children;
};
