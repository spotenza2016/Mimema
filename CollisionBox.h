#pragma once
#include "CollisionObject.h"

struct CollisionBox : CollisionObject {
    glm::vec3 position{};
    glm::vec3 size{};

    CollisionBox(const glm::vec3& position, const glm::vec3& size);

private:
    void generateBox();
};
