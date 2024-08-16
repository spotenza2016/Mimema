#pragma once
#include "Object.h"

class PhysicsObject : public Object {
    glm::vec3 position = glm::vec3(0, 0, 0);
public:
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, -9.8, 0);
    void passTime(float deltaT);

    const glm::vec3& getPosition();
    void setPosition(const glm::vec3& position);
    virtual void collisionOccurred();
};