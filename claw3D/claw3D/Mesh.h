#pragma once

class Mesh
{
public:
    Mesh(const float* vertices, unsigned int vertexCount);
    ~Mesh();

    void draw() const;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_vertexCount = 0;
};
