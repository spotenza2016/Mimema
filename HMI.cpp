#include "HMI.h"

Mimema::HMI::HMI(GLFWwindow* window) {
    glfwGetCursorPos(window, &prevCursorPosX, &prevCursorPosY);
}

void Mimema::HMI::moveCameraWithKeyboard(GLFWwindow* window, Camera::CameraState& cameraState, float deltaT) {
    glm::vec3 movement = {0, 0, 0};

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movement += glm::vec3(0, 0, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movement += glm::vec3(0, 0, -1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        movement += glm::vec3(1, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        movement += glm::vec3(-1, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        movement += glm::vec3(0, 1, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        movement += glm::vec3(0, -1, 0);
    }

    if (movement != glm::vec3(0)) {
        movement = glm::normalize(movement);
    }

    glm::vec3 upVec = glm::normalize(cameraState.upVec);
    glm::vec3 forwardVec = glm::normalize(cameraState.cameraTarget - cameraState.cameraPosition);
    glm::vec3 sideVec = glm::cross(upVec, forwardVec);
    if (sideVec != glm::vec3(0)) {
        sideVec = glm::normalize(sideVec);
    }

    movement *= keyboardSpeed * deltaT;

    upVec *= movement.y;
    forwardVec *= movement.z;
    sideVec *= movement.x;

    cameraState.cameraPosition += upVec + forwardVec + sideVec;
    cameraState.cameraTarget += upVec + forwardVec + sideVec;
}

void Mimema::HMI::moveCameraWithMouse(GLFWwindow* window, Camera::CameraState& cameraState) {
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    glm::vec2 mouseMovement = glm::vec2((float)(cursorX - prevCursorPosX), (float)(cursorY - prevCursorPosY));
    float yaw = mouseMovement.x * cursorSensitivity;

    float pitch = mouseMovement.y * cursorSensitivity;
    glm::vec3 direction = glm::normalize(cameraState.cameraTarget - cameraState.cameraPosition);
    glm::vec3 up = glm::normalize(cameraState.upVec);
    glm::mat4 rotationMatrix = glm::rotate(yaw, glm::vec3(0, -1, 0))
                               * glm::rotate(pitch, glm::cross(up, direction));
    direction = glm::normalize(rotationMatrix * glm::vec4(direction, 0));
    cameraState.cameraTarget = cameraState.cameraPosition + direction;
    up = rotationMatrix * glm::vec4(up, 0);
    cameraState.upVec = up;
    prevCursorPosY = cursorY;
    prevCursorPosX = cursorX;
}