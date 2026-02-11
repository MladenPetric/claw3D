#pragma once

#include <vector>
#include "GameObject.h"

class Shader;
class Camera;

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    void addObject(const GameObject& object);
    void update(float dt);
    void draw(const Shader& shader, const Camera& camera);

private:
    std::vector<GameObject> m_objects;
};
