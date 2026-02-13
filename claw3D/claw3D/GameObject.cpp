#include <glad/glad.h>
#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

GameObject::GameObject(Mesh* mesh, std::string name_)
    : name(std::move(name_)), m_mesh(mesh)
{
}

void GameObject::setParent(GameObject* newParent)
{
    if (m_parent == newParent) return;

    m_parent = newParent;
    transform.setParent(newParent ? &newParent->transform : nullptr);
}

void GameObject::addChild(GameObject* child)
{
    if (!child) return;
    child->setParent(this);
    m_children.push_back(child);
}

void GameObject::draw(const Shader& shader, const Camera& camera) const
{
    if (!active || !m_mesh) return;

    const glm::mat4 model = transform.getWorldMatrix();
    const glm::mat4 view = camera.getView();
    const glm::mat4 proj = camera.getProjection();

    shader.use();
    shader.setMat4("u_Model", model);
    shader.setMat4("u_View", view);
    shader.setMat4("u_Projection", proj);

    shader.setVec3("u_ObjectColor", color);

    // tekstura
    shader.setInt("u_UseTexture", (useTexture && texture != 0) ? 1 : 0);
    if (useTexture && texture != 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.setInt("u_Texture", 0);
    }

    m_mesh->draw();
}

