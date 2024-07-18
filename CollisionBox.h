#pragma once
#include "glm/vec3.hpp"
#include <algorithm>

struct CollisionBox {
    glm::vec3 position;
    glm::vec3 size;

    CollisionBox(glm::vec3 position, glm::vec3 size) {
        this->position = position;
        this->size = size;
    }

    bool intersects(const CollisionBox& rhs);
};