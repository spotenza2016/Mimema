#include "PhysicsObject.h"
void PhysicsObject::passTime(float deltaT) {
    velocity += acceleration * deltaT;
    setPosition(position + velocity * deltaT);
}

const glm::vec3 &PhysicsObject::getPosition() {
    return position;
}

void PhysicsObject::setPosition(const glm::vec3& position) {
    this->position = position;
    model->getModelState().setTranslate(position);
}

void PhysicsObject::collisionOccurred() {
    velocity = {0, 0, 0};
}
