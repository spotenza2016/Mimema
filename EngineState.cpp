#include "EngineState.h"

EngineState::EngineState() {
    camera.getCameraState().setAspectRatio((float)600 / (float)600);
    PhysicsObject* objectOne = new PhysicsObject();
    objectOne->setModel("TestFiles/oliveotter.obj");
    objectOne->setPosition(glm::vec3(0, 20, 20));
    objects.push_back(objectOne);
    PhysicsObject* character = new PhysicsObject();
    character->setModel("TestFiles/Secal.obj");
    character->setPosition(glm::vec3(0, 0, 40));
    objects.push_back(character);
    BounceObject* objectTwo = new BounceObject();
    objectTwo->setModel("TestFiles/sphere.obj");
    objectTwo->setPosition(glm::vec3(-20, 15, 0));
    objectTwo->model->getModelState().setScale({0.25, 0.25, 0.25});
    objects.push_back(objectTwo);
    // todo rn errors out if do set position before model, maybe do proper error h andling?
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

    // todo willneed to optimize vbo stuff for higher numbers (rn 60 is good and shows diff)
    for (int i = 0; i < 60; i++) {
        BounceObject* curr = new BounceObject();
        curr->setModel("TestFiles/sphere.obj");
        curr->setPosition(glm::vec3(-90, 15, -90 + (2 * i)));
        curr->model->getModelState().setScale({0.05, 0.05, 0.05});
        objects.push_back(curr);
    }

    // todo clean up plus weirdness w/ default model matrix...
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
