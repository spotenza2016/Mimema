#pragma once
#include "PhysicsObject.h"
struct BounceObject : public PhysicsObject {
    BounceObject() {
        float angle = (float)(rand() % 360);
        velocity = 25 * glm::vec3(cos(glm::radians(angle)), 0, sin(glm::radians(angle)));
    }

    void collisionOccurred();
};
