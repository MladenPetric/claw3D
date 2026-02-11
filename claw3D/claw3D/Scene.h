#pragma once
#include <vector>
#include <memory>
#include <string>

class GameObject;
class Shader;
class Camera;
class Mesh;

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    GameObject* createObject(Mesh* mesh, const std::string& name = "");

    void update(float dt);
    void draw(const Shader& shader, const Camera& camera);

private:
    std::vector<std::unique_ptr<GameObject>> m_objects;
};
