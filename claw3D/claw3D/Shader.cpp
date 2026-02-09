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
    const char* c = src.c_str();
    glShaderSource(id, 1, &c, nullptr);
    glCompileShader(id);

    int ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetShaderInfoLog(id, 1024, nullptr, log);
        std::cerr << log << std::endl;
    }

    return id;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vsrc = loadFile(vertexPath);
    std::string fsrc = loadFile(fragmentPath);

    unsigned int vs = compile(GL_VERTEX_SHADER, vsrc);
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fsrc);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, fs);
    glLinkProgram(m_ID);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
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
