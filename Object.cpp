#include "Object.h"
void Object::setModel(string objFileName) {
    delete model;
    model = new Model(objFileName);
}

Object::~Object() {
    delete model;
}
