#include "Object.h"
void Object::setModel(string objFileName) {
    delete model;
    model = new Model(objFileName);
}

Object::~Object() {
    delete model;
}

CollisionObject* Object::getCollision() {
    if (model == nullptr) {
        return nullptr;
    }

    return &model->getCollision();
}
