#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

GameObject::GameObject(Mesh* mesh)
    : m_mesh(mesh)
{
}

void GameObject::draw(const Shader& shader, const Camera& camera) const
{
    glm::mat4 mvp =
        camera.getProjection() *
        camera.getView() *
        transform.getMatrix();

    shader.use();
    shader.setMat4("u_MVP", mvp);

    m_mesh->draw();
}
