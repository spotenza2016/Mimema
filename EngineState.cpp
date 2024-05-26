#include "EngineState.h"

EngineState::EngineState() {
    camera.getCameraState().setAspectRatio((float)600 / (float)600);
    PhysicsObject* objectOne = new PhysicsObject();
    objectOne->setModel("TestFiles/oliveotter.obj");
    objectOne->setPosition(glm::vec3(0, 0, 20));
    objects.push_back(objectOne);
    Object* character = new Object();
    character->setModel("TestFiles/Secal.obj");
    character->model->getModelState().setTranslate(glm::vec3(0, 0, 40));
    objects.push_back(character);
    Object* objectTwo = new Object();
    objectTwo->setModel("TestFiles/sphere.obj");
    objectTwo->model->getModelState().setTranslate(glm::vec3(-20, 0, 0));
    objects.push_back(objectTwo);
    Object* objectThree = new Object();
    objectThree->setModel("TestFiles/plane.obj");
    objectThree->model->getModelState().setTranslate(glm::vec3(0, -20, 0));
    objects.push_back(objectThree);
    Object* objectFour = new Object();
    objectFour->setModel("TestFiles/plane.obj");
    objectFour->model->getModelState().setAngleX(glm::radians(90.0));
    objectFour->model->getModelState().setAngleY(glm::radians(180.0));
    objectFour->model->getModelState().setTranslate(glm::vec3(0, 79, 99));
    objects.push_back(objectFour);
    Object* objectFive = new Object();
    objectFive->setModel("TestFiles/plane.obj");
    objectFive->model->getModelState().setAngleX(glm::radians(90.0));
    objectFive->model->getModelState().setTranslate(glm::vec3(0, 79, -99));
    objects.push_back(objectFive);
    Object* objectSix = new Object();
    objectSix->setModel("TestFiles/plane.obj");
    objectSix->model->getModelState().setAngleX(glm::radians(90.0));
    objectSix->model->getModelState().setAngleY(glm::radians(90.0));
    objectSix->model->getModelState().setTranslate(glm::vec3(-99, 79, 0));
    objects.push_back(objectSix);
    Object* objectSeven = new Object();
    objectSeven->setModel("TestFiles/plane.obj");
    objectSeven->model->getModelState().setAngleX(glm::radians(90.0));
    objectSeven->model->getModelState().setAngleY(glm::radians(-90.0));
    objectSeven->model->getModelState().setTranslate(glm::vec3(99, 79, 0));
    objects.push_back(objectSeven);

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
