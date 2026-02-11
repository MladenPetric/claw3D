#pragma once

#include "Transform.h"

class Mesh;
class Shader;
class Camera;

class GameObject
{
public:
    GameObject(Mesh* mesh);

    void draw(const Shader& shader, const Camera& camera) const;

    Transform transform;

private:
    Mesh* m_mesh;
};
