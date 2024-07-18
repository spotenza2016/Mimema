#include "Object.h"
void Object::setModel(string objFileName) {
    delete model;
    model = new Model(objFileName);
}

Object::~Object() {
    delete model;
}

CollisionBox Object::getCollision() {
    if (model == nullptr) {
        return {{0, 0, 0}, {0, 0, 0}};
    }

    return model->getCollision();
}
