#include "Scene.h"
#include "GameObject.h"
#include "Shader.h"
#include "Camera.h"

//#include <glad/glad.h>

GameObject* Scene::createObject(Mesh* mesh, const std::string& name)
{
    m_objects.push_back(std::make_unique<GameObject>(mesh, name));
    return m_objects.back().get();
}

void Scene::update(float)
{
    // kasnije: state machine, animacije, collision, itd.
}

void Scene::draw(const Shader& shader, const Camera& camera)
{
    // prvo solid
    for (auto& obj : m_objects)
    {
        if (!obj->transparent)
        {
            shader.setFloat("u_Alpha", 1.0f);
            obj->draw(shader, camera);
        }
    }

    // onda transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto& obj : m_objects)
    {
        if (obj->transparent)
        {
            shader.setFloat("u_Alpha", 0.3f);
            obj->draw(shader, camera);
        }
    }

    glDisable(GL_BLEND);
}

