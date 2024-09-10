#include "Object.h"
Mimema::Object::ObjectState Mimema::Object::ObjectState::operator*(float alpha) const {
    return {translate * alpha,
            angles * alpha,
            scale * alpha};
}

Mimema::Object::ObjectState Mimema::Object::ObjectState::operator+(const Mimema::Object::ObjectState& rhs) const {
    return {translate + rhs.translate,
            angles + rhs.angles,
            scale + rhs.scale};
}

void Mimema::Object::updateCollider() {
    if (model != nullptr) {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1), objectState.translate);
        glm::mat4 rotationMatrixX = glm::rotate(objectState.angles.x, glm::vec3(1, 0, 0));
        glm::mat4 rotationMatrixY = glm::rotate(objectState.angles.y, glm::vec3(0, 1, 0));
        glm::mat4 rotationMatrixZ = glm::rotate(objectState.angles.z, glm::vec3(0, 0, 1));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), objectState.scale);

        glm::mat4 modelMatrix = translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * scaleMatrix;

        collider = Collider(model->getBBPos(), model->getBBSize(), modelMatrix);
    }
    else {
        collider = Collider();
    }
}

Mimema::Object::Object(Mimema::Renderable* model) {
    this->model = model;

    updateCollider();
}

const glm::vec3& Mimema::Object::getTranslate() {
    return objectState.translate;
}

const glm::vec3& Mimema::Object::getAngles() {
    return objectState.angles;
}

const glm::vec3& Mimema::Object::getScale() {
    return objectState.scale;
}

const Mimema::Collider& Mimema::Object::getCollider() const {
    return collider;
}

void Mimema::Object::setTranslate(const glm::vec3& translate) {
    objectState.translate = translate;
    updateCollider();
}

void Mimema::Object::addToTranslate(const glm::vec3& add) {
    objectState.translate += add;
    updateCollider();
}

void Mimema::Object::setAngles(const glm::vec3& angles) {
    objectState.angles = angles;
    updateCollider();
}

void Mimema::Object::addToAngles(const glm::vec3& add) {
    objectState.angles += add;
    updateCollider();
}

void Mimema::Object::setScale(const glm::vec3& scale) {
    objectState.scale = scale;
    updateCollider();
}

void Mimema::Object::addToScale(const glm::vec3& add) {
    objectState.scale += add;
    updateCollider();
}

void Mimema::Object::setModelMatrix(const glm::vec3& translate, const glm::vec3& angles, const glm::vec3& scale) {
    objectState.translate = translate;
    objectState.angles = angles;
    objectState.scale = scale;

    updateCollider();
}

void Mimema::Object::addToModelMatrix(const glm::vec3& addTranslate, const glm::vec3& addAngles, const glm::vec3& addScale) {
    objectState.translate += addTranslate;
    objectState.angles += addAngles;
    objectState.scale += addScale;

    updateCollider();
}

const Mimema::Object::ObjectState& Mimema::Object::getObjectState() {
    return objectState;
}

void Mimema::Object::passTime(float deltaT) {
    if (physicsMask == 0) {
        return;
    }

    addToTranslate(velocity * deltaT);
    velocity += acceleration * deltaT;
}

void Mimema::Object::collisionOccurred() {
    velocity = {0, 0, 0};
}