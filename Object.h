#pragma once
#include <string>
#include "Model.h"
using namespace std;

// Top Level Object Class
class Object {
public:
    Model* model = nullptr;

    // Set Model for this Object
    void setModel(string objFileName);

    // Destructor
    virtual ~Object();

    CollisionObject& getCollision();
};
