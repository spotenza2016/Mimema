#ifndef MIMEMA_HMI_H
#define MIMEMA_HMI_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tracy/Tracy.hpp"

#include "Level.h"

namespace Mimema {
    class HMI {
        float cursorSensitivity = 1.0f/1000.0f;
        float keyboardSpeed = 40;
        double prevCursorPosX;
        double prevCursorPosY;
    public:
        explicit HMI(GLFWwindow* window);
        void moveCameraWithKeyboard(GLFWwindow* window, Camera::CameraState& cameraState, float deltaT);
        void moveCameraWithMouse(GLFWwindow* window, Camera::CameraState& camera);
    };
}

#endif //MIMEMA_HMI_H