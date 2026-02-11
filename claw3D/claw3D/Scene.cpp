#include "Scene.h"
#include "Shader.h"
#include "Camera.h"

void Scene::addObject(const GameObject& object)
{
    m_objects.push_back(object);
}

void Scene::update(float)
{
    // kasnije animacije
}

void Scene::draw(const Shader& shader, const Camera& camera)
{
    for (auto& obj : m_objects)
        obj.draw(shader, camera);
}
