#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;

    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setFloat(const std::string& name, float value) const;

private:
    unsigned int m_ID = 0;

    static std::string loadFile(const std::string& path);
    static unsigned int compile(unsigned int type, const std::string& src);
};
