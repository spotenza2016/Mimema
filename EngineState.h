#pragma once
#include "Camera.h"
#include "Level.h"
#include "Object.h"
#include "PhysicsObject.h"
#include "BounceObject.h"

struct EngineState {
    Camera::CameraState cameraPrevState;
    Level::LevelState levelPrevState;
    vector<Model::ModelState> objectPrevStates;

    // Next State (find DS for these)
    // Need to ensure add/delete will work (once those are added)
    Camera camera;
    Level level;
    vector<Object*> objects;

    EngineState();
    void saveStates();
};