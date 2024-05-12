#pragma once
#include <string>
#include "OpenGlHandler.h"
#include <chrono>
#include "Level.h"
#include "Camera.h"
#include <unistd.h>
#include <windows.h>
#include <winuser.h>
using namespace std;

// Core class of the engine
class EngineCore {
public:
    // Microseconds per Frame in 60 FPS
    constexpr static double simulationFPS = 144;
    constexpr static double minimumFPS = 5;
    constexpr static double simulationDeltaT = 1.0 / simulationFPS * 1000000000;
    constexpr static double maxDeltaT = max(1.0 / minimumFPS * 1000000000, simulationDeltaT);

    // Core Loop of Engine
    void engineLoop(int width, int height, const string& name);

    // TODO move to own class
    // Keyboard controls
    double lastCursorPosX;
    double lastCursorPosY;
    void processInput(GLFWwindow* window, Camera* camera);
};