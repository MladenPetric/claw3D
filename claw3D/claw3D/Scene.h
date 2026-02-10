#pragma once

#include <vector>
#include <glm/glm.hpp>

class Shader;
class Camera;
class Mesh;

struct SceneObject
{
    Mesh* mesh;
    glm::mat4 model;
};

class Scene
{
public:
    Scene();
    ~Scene();

    void addObject(Mesh* mesh, const glm::mat4& transform);
    void update(float dt);
    void draw(const Shader& shader, const Camera& camera);

private:
    std::vector<SceneObject> m_objects;
};
