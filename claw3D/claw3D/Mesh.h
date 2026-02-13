#pragma once
#include <string>

class Mesh
{
public:
    // proceduralno: vertices su interleaved, stride je broj floatova po verteksu
    Mesh(const float* vertices, unsigned int vertexCount, unsigned int strideFloats = 6);

    // OBJ loader: pravi interleaved pos+normal+uv (8 floatova)
    Mesh(const std::string& objPath);

    ~Mesh();

    void draw() const;

private:
    void upload(const float* vertices, unsigned int vertexCount, unsigned int strideFloats);

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_vertexCount = 0;
    unsigned int m_strideFloats = 0;
};
