#pragma once
#include <string>
#include "OpenGlHandler.h"
#include <chrono>
#include "Level.h"
#include "Camera.h"
#include "Object.h"
#include "PhysicsObject.h"
#include "Octree.h"
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
using namespace std;

// Core class of the engine
class EngineCore {
    glm::vec3 position = {-200, -200, -200};
    glm::vec3 size = {400, 400, 400};
public:
    // Seconds per Frame in 60 FPS
    constexpr static double simulationFPS = 144;
    constexpr static double minimumFPS = 5;
    constexpr static double simulationDeltaT = 1.0 / simulationFPS;
    constexpr static double maxDeltaT = max(1.0 / minimumFPS, simulationDeltaT);

    // Core Loop of Engine
    void engineLoop(int width, int height, const string& name);

    void handlePhysics(EngineState& state);

    // TODO move to own class
    // Keyboard controls
    double lastCursorPosX;
    double lastCursorPosY;
    void processInput(GLFWwindow* window, Camera* camera);
};