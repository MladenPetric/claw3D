#include "Mesh.h"
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };

static void trimLeft(std::string& s)
{
    size_t i = 0;
    while (i < s.size() && std::isspace((unsigned char)s[i])) i++;
    s.erase(0, i);
}

static bool parseOBJ(
    const std::string& path,
    std::vector<float>& outInterleaved, 
    unsigned int& outVertexCount
)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;

    positions.reserve(1000);
    normals.reserve(1000);
    uvs.reserve(1000);

    outInterleaved.clear();
    outInterleaved.reserve(3000 * 8);

    auto pushVertex = [&](int vi, int ti, int ni)
        {
            auto fixIndex = [](int idx, int count) -> int {
                if (idx > 0) return idx - 1;
                if (idx < 0) return count + idx;
                return -1;
                };

            int vIndex = fixIndex(vi, (int)positions.size());
            int tIndex = fixIndex(ti, (int)uvs.size());
            int nIndex = fixIndex(ni, (int)normals.size());

            Vec3 p{ 0,0,0 };
            Vec3 n{ 0,0,1 };
            Vec2 t{ 0,0 };

            if (vIndex >= 0) p = positions[vIndex];
            if (nIndex >= 0) n = normals[nIndex];
            if (tIndex >= 0) t = uvs[tIndex];

            outInterleaved.push_back(p.x);
            outInterleaved.push_back(p.y);
            outInterleaved.push_back(p.z);

            outInterleaved.push_back(n.x);
            outInterleaved.push_back(n.y);
            outInterleaved.push_back(n.z);

            outInterleaved.push_back(t.x);
            outInterleaved.push_back(t.y);
        };

    std::string line;
    while (std::getline(file, line))
    {
        trimLeft(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            Vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        }
        else if (type == "vn")
        {
            Vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (type == "vt")
        {
            Vec2 t;
            ss >> t.x >> t.y;
            uvs.push_back(t);
        }
        else if (type == "f")
        {
            std::vector<std::string> tokens;
            tokens.reserve(8);

            std::string tok;
            while (ss >> tok) tokens.push_back(tok);
            if (tokens.size() < 3) continue;

            auto parseTriplet = [](const std::string& s, int& vi, int& ti, int& ni)
                {
                    vi = ti = ni = 0;
                    // v/vt/vn
                    size_t p1 = s.find('/');
                    if (p1 == std::string::npos)
                    {
                        vi = std::stoi(s);
                        return;
                    }
                    std::string a = s.substr(0, p1);
                    vi = a.empty() ? 0 : std::stoi(a);

                    size_t p2 = s.find('/', p1 + 1);
                    if (p2 == std::string::npos)
                    {
                        // v/vt
                        std::string b = s.substr(p1 + 1);
                        ti = b.empty() ? 0 : std::stoi(b);
                        return;
                    }

                    std::string b = s.substr(p1 + 1, p2 - (p1 + 1));
                    std::string c = s.substr(p2 + 1);              
                    ti = b.empty() ? 0 : std::stoi(b);
                    ni = c.empty() ? 0 : std::stoi(c);
                };

            int v0, t0, n0;
            parseTriplet(tokens[0], v0, t0, n0);

            for (size_t i = 1; i + 1 < tokens.size(); i++)
            {
                int v1, t1, n1;
                int v2, t2, n2;
                parseTriplet(tokens[i], v1, t1, n1);
                parseTriplet(tokens[i + 1], v2, t2, n2);

                pushVertex(v0, t0, n0);
                pushVertex(v1, t1, n1);
                pushVertex(v2, t2, n2);
            }
        }
    }

    outVertexCount = (unsigned int)(outInterleaved.size() / 8);
    return outVertexCount > 0;
}

Mesh::Mesh(const float* vertices, unsigned int vertexCount, unsigned int strideFloats)
{
    upload(vertices, vertexCount, strideFloats);
}

Mesh::Mesh(const std::string& objPath)
{
    std::vector<float> verts;
    unsigned int vc = 0;

    if (!parseOBJ(objPath, verts, vc))
    {
        m_vertexCount = 0;
        return;
    }

    upload(verts.data(), vc, 8);
}

void Mesh::upload(const float* vertices, unsigned int vertexCount, unsigned int strideFloats)
{
    m_vertexCount = vertexCount;
    m_strideFloats = strideFloats;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        (size_t)vertexCount * strideFloats * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        strideFloats * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        strideFloats * sizeof(float),
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    if (strideFloats >= 8)
    {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
            strideFloats * sizeof(float),
            (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::draw() const
{
    if (!m_VAO || m_vertexCount == 0) return;
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}
