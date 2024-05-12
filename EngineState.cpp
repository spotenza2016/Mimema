#include "EngineState.h"

EngineState::EngineState() {
    camera.getCameraState().setAspectRatio((float)600 / (float)600);
    Object* objectOne = new Object();
    objectOne->setModel("TestFiles/oliveotter.obj");
    objectOne->model->getModelState().setTranslate(glm::vec3(0.5, 0, 0));
    objects.push_back(objectOne);
    Object* objectTwo = new Object();
    objectTwo->setModel("TestFiles/sphere.obj");
    objectTwo->model->getModelState().setTranslate(glm::vec3(-0.5, 0, 0));
    objects.push_back(objectTwo);
    Object* objectThree = new Object();
    objectThree->setModel("TestFiles/plane.obj");
    objectThree->model->getModelState().setTranslate(glm::vec3(0, -0.1, 0));
    objects.push_back(objectThree);

    objectPrevStates.resize(objects.size());

    saveStates();
}

void EngineState::saveStates() {
    cameraPrevState = camera.getCameraState();
    levelPrevState = level.getLevelState();

    for (int i = 0; i < objects.size(); i++) {
        objectPrevStates.at(i) = objects.at(i)->model->getModelState();
    }
}
