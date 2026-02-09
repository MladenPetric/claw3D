#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const float* vertices, unsigned int vertexCount)
    : m_vertexCount(vertexCount)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexCount * 3 * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::draw() const
{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}
