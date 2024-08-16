#include "CollisionBox.h"

void CollisionBox::generateBox() {
    vertices = {position,
                position + glm::vec3(size.x, 0, 0),
                position + glm::vec3(0, 0, size.z),
                position + glm::vec3(size.x, 0, size.z),
                position + glm::vec3(0, size.y, 0),
                position + glm::vec3(size.x, size.y, 0),
                position + glm::vec3(0,size.y, size.z),
                position + size};
    faces = {1, 3, 2,
                2, 3, 4,
                1, 2, 5,
                2, 6, 5,
                3, 7, 4,
                4, 7, 8,
                1, 5, 3,
                3, 5, 7,
                2, 4, 6,
                4, 8, 6,
                5, 6, 7,
                6, 8, 7};
}

CollisionBox::CollisionBox(const glm::vec3& position, const glm::vec3& size) {
    this->position = position;
    this->size = size;
    generateBox();
}