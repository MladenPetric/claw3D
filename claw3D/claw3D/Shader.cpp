#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

std::string Shader::loadFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

unsigned int Shader::compile(unsigned int type, const std::string& src)
{
    unsigned int id = glCreateShader(type);
    const char* csrc = src.c_str();
    glShaderSource(id, 1, &csrc, nullptr);
    glCompileShader(id);

    int ok = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetShaderInfoLog(id, 1024, nullptr, log);
        std::cerr << "Shader compile error:\n" << log << "\n";
    }
    return id;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vsSrc = loadFile(vertexPath);
    std::string fsSrc = loadFile(fragmentPath);

    unsigned int vs = compile(GL_VERTEX_SHADER, vsSrc);
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fsSrc);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, fs);
    glLinkProgram(m_ID);

    int ok = 0;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetProgramInfoLog(m_ID, 1024, nullptr, log);
        std::cerr << "Program link error:\n" << log << "\n";
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    if (m_ID) glDeleteProgram(m_ID);
}

void Shader::use() const
{
    glUseProgram(m_ID);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    int loc = glGetUniformLocation(m_ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    int loc = glGetUniformLocation(m_ID, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string& name, float value) const
{
    int loc = glGetUniformLocation(m_ID, name.c_str());
    glUniform1f(loc, value);
}
void Shader::setInt(const std::string& name, int value) const
{
    int loc = glGetUniformLocation(m_ID, name.c_str());
    glUniform1i(loc, value);
}
