#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

Scene::Scene() {}
Scene::~Scene() {}

void Scene::addObject(Mesh* mesh, const glm::mat4& transform)
{
    m_objects.push_back({ mesh, transform });
}

void Scene::update(float)
{
    // kasnije: animacije, logika, kretanja
}

void Scene::draw(const Shader& shader, const Camera& camera)
{
    shader.use();

    for (auto& obj : m_objects)
    {
        glm::mat4 mvp =
            camera.getProjection() *
            camera.getView() *
            obj.model;

        shader.setMat4("u_MVP", mvp);
        obj.mesh->draw();
    }
}
