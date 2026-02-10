#include "GameObject.h"

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(Mesh* mesh)
    : m_mesh(mesh),
    m_position(0.0f),
    m_rotation(0.0f),
    m_scale(1.0f)
{
}

void GameObject::setPosition(const glm::vec3& pos)
{
    m_position = pos;
}

void GameObject::setRotation(const glm::vec3& rot)
{
    m_rotation = rot;
}

void GameObject::setScale(const glm::vec3& scale)
{
    m_scale = scale;
}

void GameObject::draw(const Shader& shader, const Camera& camera) const
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, m_position);

    model = glm::rotate(model, m_rotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, m_rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, m_rotation.z, glm::vec3(0, 0, 1));

    model = glm::scale(model, m_scale);

    glm::mat4 mvp =
        camera.getProjection() *
        camera.getView() *
        model;

    shader.use();
    shader.setMat4("u_MVP", mvp);

    m_mesh->draw();
}
