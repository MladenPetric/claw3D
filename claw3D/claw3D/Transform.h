#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>

class Transform
{
public:
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f }; // stepeni: pitch(x), yaw(y), roll(z)
    glm::vec3 scale{ 1.0f };

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::mat4 getLocalMatrix() const
    {
        glm::mat4 model(1.0f);

        model = glm::translate(model, position);

        model = glm::rotate(model, glm::radians(rotation.x), { 1,0,0 });
        model = glm::rotate(model, glm::radians(rotation.y), { 0,1,0 });
        model = glm::rotate(model, glm::radians(rotation.z), { 0,0,1 });

        model = glm::scale(model, scale);
        return model;
    }

    glm::mat4 getWorldMatrix() const
    {
        if (parent)
            return parent->getWorldMatrix() * getLocalMatrix();
        return getLocalMatrix();
    }

    void setParent(Transform* newParent)
    {
        if (parent == newParent) return;

        // izbaci iz starog parenta
        if (parent)
        {
            auto& siblings = parent->children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }

        parent = newParent;

        // ubaci u novog parenta
        if (parent)
            parent->children.push_back(this);
    }
};
