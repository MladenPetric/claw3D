#pragma once
#include <string>

class Shader
{
public:
    Shader(const char* vertPath, const char* fragPath);
    ~Shader();

    void use() const;

private:
    unsigned int m_id;
    unsigned int compile(unsigned int type, const std::string& src);
    std::string loadFile(const char* path);
};

