#include "Object.h"
void Object::setModel(string objFileName) {
    delete model;
    model = new Model(objFileName);
}

Object::~Object() {
    delete model;
}

CollisionObject& Object::getCollision() {
    // todo handle this
    if (model == nullptr) {

    }

    // TODO make this collision object stored inside and just change when changed. rn calculates it more times than needed
    return model->getCollision();
}
